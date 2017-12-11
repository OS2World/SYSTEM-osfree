/*
 * Copyright (C) 1999-2001, The University of Queensland
 * Copyright (C) 1999-2001, Sun Microsystems, Inc
 *
 * See the file "LICENSE.TERMS" for information on usage and
 * redistribution of this file, and for a DISCLAIMER OF ALL
 * WARRANTIES.
 *
 */

/*==============================================================================
 * FILE:        outfile.cc
 * OVERVIEW:    This file contains the implementation of the OutputFile class.
 *              Also writes the makefile
 *              Formerly, it managed the various assembler files resulting from
 *              running VPO (Very Portable Optimiser) over various .cex files,
 *              and handled assembling and linking them
 *
 * Copyright (C) 1999-2001, The University of Queensland, BT group
 * Copyright (C) 1999-2001, Sun Microsystems, Inc
 *============================================================================*/

/*
 * $Revision: 1.21 $
 * 27 May 99 - Mike: Now link with link mapfiles, instead of using objcopy
 * 01 Jun 99 - Mike: Handle .c as well as .s files
 * 23 Jul 99 - Mike: added "make data" for uqbtj
 * 12 Aug 99 - Mike: replaced "make data" with "make java"
 * 27 Aug 99 - Mike: Added cpp preprocessor
 * 08 Dec 99 - Mike: Solved the "dynamic global" problem with the changerel
 *              utility (generates _global.s and _global.chg as well)
 *  9 Dec 99 - CC: Changes to Makefile for JVM, order of files and commented 
 *              out references to bssdata as "head" doesn't work on 
 *              Solaris in the same way as Linux. 
 * 13 Dec 99 - CC: GNU head works ok, Sun's head doesn't.
 * 06 Jan 00 - Mike: Changed .word to .long (all assemblers seem to handle this)
 * 16 Feb 00 - Mike: Mods to cope with non elf input files
 * 21 Mar 00 - Mike: OutputFile::link() outputs _global[] declaration 
 *             Cristina: the generated Makefile now includes _globals.o
 * 03 Jul 00 - Mike: generate _runtime.h (etc) if needed
 * 31 Jul 00 - Cristina: Fixed path to uqbt.h
 * 22 Aug 00 - Mike: fixed sorting of names for the address map; used to
 *              segfault if the loader didn't have a name for an address
 * 24 Aug 00 - Mike: Append prototypes for user procedures to uqbt.h
 * 24 Aug 00 - Mike: Also don't emit functions like _umul to the address map
 * 22 Sep 00 - Mike: make clean was removing Targetname.* (removes .map file
 *              if source has initial capital)
 * 13 Nov 00 - Mike: Reversed sense of -y switch
 * 16 Nov 00 - Brian: Replaced support for old gcc-based JVM backend with that
 *              for the new JVM backend. Added "make all" rule to generated 
 *              Makefile and put in some better pathnames for tools.
 * 22 Mar 01 - Mike: Append the ints2float helper function to uqbt.h
 * 10 Apr 01 - Mike: Removed `inline' keyword from def of ints2float (for cc)
 * 11 Apr 01 - Mike: map renamed to _map to avoid name collisions
 * 16 Apr 01 - Brian: "make clean" must not delete .o files generated by the
 *              PostOptimizer backend.
 * 19 Apr 01 - Mike: Process the -l argument, i.e. libraries for the target 
 * 24 Apr 01 - Brian: Added support for VPO back end.
 * 06 May 01 - Mike: Don't add functions starting with "__" to the map
 * 31 May 01 - Brian: VPO sometimes generates V9 instructions such as fmovd
 *              so we request "as" to assume the V9 architecture.
 * 17 Jun 01 - Brian: For VPO, only request "as" to assume the V9 architecture
 *              if generating code for the SPARC.
 * 24 Jul 01 - Brian: Added ARM VPO support.
 * 01 Aug 01 - Mike: Slight change to the declaration of _globals
 * 08 Aug 01 - Mike: Changed the way the _globals files are written in a source
 *              independant way, using GetDynamicGlobalMap()
 * 23 Aug 01 - Cristina: added a path for "head" and removed obsolete code for 
 *		determining paths
 * 28 Aug 01 - Mike: "namespace" declaration protected by "#if __GNUC__"
 * 05 Sep 01 - Brian: Added emitVPOUtilFile() procedure that is called when
 *              using a VPO backend. Also made the actual program name for
 *              changerel in the generated Makefile a parameter that can be
 *              overridden on the make command line.
 * 06 Sep 01 - Brian: Don't emit the VPO utility file _util.c if
 *              progOptions.noendian is set.
 * 12 Sep 01 - Mike: Check for nonzero symbol name before underscore test
 */

#include "global.h"             // For prog object etc
#include "prog.h"               // For program globals
#include "proc.h"               // For getName() etc
#include "options.h"            // For progOptions.outDir etc
#include "outfile.h"            // For the OutputFile class
#include "ElfBinaryFile.h"      // For SectionInfo etc
#include "PalmBinaryFile.h"     // For Palm specific calls
#include "backend.h"            // At least for legalCname()

void error(const string& Msg);  // In error.cc

OutputFile::OutputFile()
{
}

/*==============================================================================
 * FUNCTION:    OutputFile::addSource
 * OVERVIEW:    Add a name to the list of source files (with extension, e.g.
 *                  main.c or data.data.s)
 * PARAMETERS:  name: procedure name as above
 * RETURNS:     Nothing
 *============================================================================*/
void OutputFile::addSource(const string& name)
{
    srcNames.push_back(name);
}

#if 0
/*==============================================================================
 * FUNCTION:    OutputFile::addData
 * OVERVIEW:    Add a struct to the list of structs describing data sections.
 * PARAMETERS:  name: procedure name as above
 * RETURNS:     Nothing
 *============================================================================*/
void OutputFile::addData(const DATA_INFO& id)
{
    dataInfo.push_back(id);
}
#endif

/*==============================================================================
 * FUNCTION:    OutputFile::link
 * OVERVIEW:    Create the output file, given all the procedure source files
 *                  and data files that have been added
 *              Sends commands to Makefile now
 * PARAMETERS:  bBlockMoveReqd: true if using the "block move required" plan.
 *                  This means also adding "-e uqbt_start" to the final link,
 *                  so that code gets control before _start
 *              sizeBSSdata: size of the .bss section in bytes, or 0 if none.
 * RETURNS:     true if successful
 *============================================================================*/
bool OutputFile::link(bool bBlockMoveReqd, int sizeBSSdata)
{
    list<string>::iterator it;

    ofstream mf;
    mf.open((progOptions.outDir + "Makefile").c_str());
    if (mf == NULL) {
        error("Could not open Makefile for writing");
        return false;
    }

    mf << "CC = " << TARGET_CC_PATH << " -w -O4\n";
    mf << "AS = " << TARGET_AS_PATH << "\n";
    mf << "LD = " << TARGET_CC_PATH << "\n";
    mf << "OBJCOPY = " << TARGET_OBJCOPY_PATH << "\n";
    mf << "JASMIN = " << TARGET_JASMIN_PATH << "\n";
    mf << "HEAD = " << TARGET_HEAD_PATH << "\t#make sure you use GNU's head\n";
    mf << "VPO = " << TARGET_VPO_PATH << "\n";
#ifdef TARGET_OBJRES_PATH
    mf << "OR = " << TARGET_OBJRES_PATH << "\n";
#endif
#ifdef TARGET_BUILDPRC_PATH
    mf << "BP = " << TARGET_BUILDPRC_PATH << "\n";
#endif
    mf << "CHANGEREL = changerel\n";
    
    //  //  //  //  //  //  //  //  //  //
    //                                  //
    //   D y n a m i c   G l o b a l s  //
    //                                  //
    //  //  //  //  //  //  //  //  //  //

    // Check if have dynamic relocatable objects
    bool dynGlobals = false;
    map<ADDRESS, const char*>* dmap = prog.pBF->GetDynamicGlobalMap();
    // All gcc compiled programs have at least one entry here; most of the time
    // it's _environ, which is likely not used anyway. So there is the -y switch
    // to allow us to not worry about global dynamics when there is only one
    int numDyn = 0;
    if (dmap) numDyn = dmap->size();
    if ((numDyn > 1) || ((numDyn == 1) && !progOptions.dynamicGlobal1)) {
        cout << "Source has " << numDyn << " dynamic symbol" <<
            ((numDyn > 1) ? "s" : "") << endl;
        dynGlobals = true;
        // We need to create appropriate rel[a].bss section entries in
        // the target binary. After much research, the best way seems to be to
        // add an assembler source file to the make for the target, which uses
        // each symbol. They will be undefined, so the linker will create them
        // in the bss, and put an entry in the target's rel[a].bss section, but
        // the addresses will be wrong. We change the addresses with the stand-
        // alone utility changerel (we copy a link to that utility to the output
        // directory). So we need to generate two files here; one is an assembly
        // source file, and the other is the change list file (_globals.chg).
        ofstream af((progOptions.outDir + "_globals.s").c_str());
        if (!af) {
            error("Could not create assembly file _globals.s");
            return false;
        }
        ofstream cf((progOptions.outDir + "_globals.chg").c_str());
        if (!cf) {
            error("Could not create global change list file _globals.chg");
            return false;
        }

        map<ADDRESS, const char*>::iterator ii;
        for (ii = dmap->begin(); ii != dmap->end(); ii++) {
            const char* s = ii->second;
            ADDRESS val = ii->first;
            // Each entry in this file is just ".long <sym>"
            af << ".long " << s << endl;
            // Each entry in this file is of the form "<sym> <address>"
            cf << s << " " << dec << val << endl;
        }
        af.close();
        cf.close();

        // Add _globals.s to the list of source files
        addSource("_globals.s");
    }

    // Copy uqbt.s if needed: cp TRANSTIME/<TGTLNAME>.uqbt.s <outdir>/uqbt.s
    // Note: at present, this is only needed when a register call is found,
    // and we are not using -s or -S
    if (prog.bRegisterCall && !progOptions.start) {
        ostrstream cmd;
        cmd << "cp " << prog.getProgPath() << "backend/runtime/" << TGTLNAME <<
            ".uqbt.s ";
        cmd << progOptions.outDir << "uqbt.s";
        int ret = system(str(cmd));
        if (ret) {
            error("Could not copy uqbt.s");
            return false;
        }
        // Also copy _runtime.c
        ostrstream cmd2;
        cmd2 << "cp " << prog.getProgPath() << "backend/runtime/_runtime.c " <<
            progOptions.outDir;
        ret = system(str(cmd2));
        if (ret) {
            error("Could not copy _runtime.c");
            return false;
        }
        // Generate _runtime.h
        if (genRuntimeHdr())
            return false;
        // Also add uqbt.s as a source file, and _runtime.c
        addSource("uqbt.s");
        addSource("_runtime.c");
    }

#if SRCENDIAN != TGTENDIAN
    // If using the VPO backend, add the _util.c file to the list of source
    // files. This file contains definitions of swap routines.
    if (progOptions.translateToVPO && !progOptions.noendian) {
        addSource("_util.c");
    }
#endif   

    // Make a label for all the object files
    // <prog>.uqbt: main.o proc1.o ... data.rodata.o ...
    mf << "OBJS = ";
    for (it = srcNames.begin(); it != srcNames.end(); it++) {
        mf << changeExt(*it, ".o") << " ";
    }
    mf << "\n\n";

    // Add "make all" rule to make both the C and the JVM programs
    mf << "all:\t " << prog.getNameNoPath() << "\n";

    // Main dependency: <prog>: ${OBJS}
    mf << endl << prog.getNameNoPath() << ": ${OBJS}\n";

    // Now how to link the object files to an executable
#if TGT == MC68K
    // ${CC} -o <prog> ${OBJS}
    // Get the name without the ".prc"
    string baseName(changeExt(prog.getNameNoPath(), ""));
    mf << "\t${CC} -o " << baseName << " ${OBJS}\n";
    mf << "\t${OR} " << baseName << endl;
    // Example: ${BP} StarterUqbt.prc 'StarterUqbt' 1937011316 *.grc *.bin
    mf << "\t${BP} " << prog.getNameNoPath() << " '" << baseName << "' ";
    mf << dec << ((PalmBinaryFile*)prog.pBF)->GetAppID() << " *.grc *.bin\n";
#else
    // Note: the -Wl,xyz switch passes xyz to the linker. 
    // For Solaris, we use -Mfoo.map, which says to use foo.map as the mapfile.
    // For Linux, we use the equivalent -Tfoo.map.
    //    ${CC} -Wl,-T<prog>.map -o <prog>.uqbt ${OBJS}  or  -Wl,-M<prog>.map
    #if __linux__
        mf << "\t${CC} -Wl,-T" << prog.getNameNoPath() << ".map -o ";
    #else
        #if TGT == ARM
            mf << "\t${CC} -Wl,-T" << prog.getNameNoPath() << ".map -o ";
        #else
            mf << "\t${CC} -Wl,-M" << prog.getNameNoPath() << ".map -o ";
        #endif
    #endif
        mf << prog.getNameNoPath() << " ${OBJS}";
        
        // Process the argument to -l, if given. This will be an instruction
        // to the linker (of the translated binary) to search these libraries
        if (progOptions.libString.length()) {
            // First remove the leading "-l"
            string ls = progOptions.libString.substr(2);
            while (ls.length()) {
                unsigned dollarPos = ls.find('$');
                if (dollarPos == string::npos) {
                    mf << " -l" << ls;
                    break;          // No more to process
                }
                else {
                    // We want to copy the string up to but not including the $
                    mf << " -l" << ls.substr(0, dollarPos-1);
                    // Delete up to and including the dollar, i.e. keep from
                    // after the dollar to the end
                    ls = ls.substr(dollarPos+1);
                }
            }
        }

        // Add "-e uqbt_start" if required
        if (bBlockMoveReqd) {
            mf << " -e uqbt_start";
        }
        mf << "\n";
#endif

    // If we have dynamic globals: "changerel <outfile> -f _globals.chg"
    if (dynGlobals) {
        mf << "\t${CHANGEREL} " << prog.getNameNoPath() << " -f _globals.chg\n";
        // Copy that changerel utility
        ostrstream cmd;
        cmd << "cp " << prog.getProgPath() << "backend/runtime/changerel " ;
        cmd << progOptions.outDir;
        int ret = system(str(cmd));
        if (ret) {
            error("Could not copy changerel utility");
        }
    }
    
    // Add entries for clean (removes .o files and exe) and clean_all
    // (also removes source files)
    mf << "\nclean:\n";
    mf << "\trm -f " << prog.getNameNoPath();
    if (progOptions.useExp) {
        // BTL 4/16/01: This is a hack. Using ${OBJS} deletes too many files.
        // It deletes the <proc>.o object files generated by the PostOptimizer
        // backend. We need some way to indicate that a .o file must not be
        // deleted.
        mf << " rodata.o rwdata.o _globals.o start.o uqbt.o _runtime.o ";
    } else {
        mf << " ${OBJS} ";
    }
    mf << prog.getNameNoPath() + ".class ";
    mf << "Uqbt.j rodata rwdata *.dec\n\n";

    // If using the VPO backend, emit commands to run VPO on each generated
    // .cex file and then assemble the resulting .s file.
    if (progOptions.translateToVPO) {
        for (it = srcNames.begin();  it != srcNames.end();  it++) {
            string srcName = (*it);
            if (hasExt(srcName, "cex")) {
                string sFile   = changeExt(srcName, ".s");
                string oFile   = changeExt(srcName, ".o");
                string decFile = changeExt(srcName, ".dec");
                mf << oFile << ": " << srcName << "\n";
                mf << "\trm -f " << decFile << "\n";
                mf << "\tvpo-decode < " << srcName << " > " << decFile << "\n";
#if TGT == ARM
                mf << "\t${VPO} -GLAO " << srcName << " " << sFile << "\n";
#else                
                mf << "\t${VPO} -GLAOFI " << srcName << " " << sFile << "\n";
#endif                
#if TGT == SPARC
                // Interestingly, VPO will sometimes generate V9 instructions
                // such as fmovd that require that we request "as" to assume
                // the V9 architecture.
                mf << "\t${AS} -Av9 -o " << oFile << " " << sFile << "\n\n";
#else
                mf << "\t${AS} -o " << oFile << " " << sFile << "\n\n";
#endif
            }
        }
    }

    //  //  //  //  //  //  //
    //                      //
    //   M a k e  j a v a   //
    //                      //
    //  //  //  //  //  //  //

    // Dependencies for rwdata, rodata, and bssdata. Only write a dependency
    // for rodata, but make all 3 at once
    mf << "rodata: rodata.o rwdata.o\n";
    // gobjcopy rwdata.o rwdata -O binary -R .text -R .data -R .bss
    mf << "\t${OBJCOPY} rodata.o rodata -O binary -R .text -R .data -R .bss\n";
    mf << "\t${OBJCOPY} rwdata.o rwdata -O binary -R .text -R .data -R .bss\n";
    mf << "\t${HEAD} -c " << dec << sizeBSSdata << " /dev/zero > bssdata\n";  

    mf << "\njava: ";
    // Depends on the class file (so it gets made)
    // Also depends on rwdata and rodata (the raw data files)
    mf << prog.getNameNoPath() << ".class rodata rwdata bssdata\n";  

    // Make the class file with jasmin
    mf << "\n" << prog.getNameNoPath() + ".class: Uqbt.j\n";
    mf << "\t${JASMIN} -g Uqbt.j";

    // Make Uqbt.j using "cat"
    mf << "\n\nUqbt.j: " << prog.getNameNoPath() + ".j readData.j\n";
    mf << "\tcat " << prog.getNameNoPath() + ".j readData.j > Uqbt.j\n";
    mf.close();
    
    // Copy uqbt.h: cp backend/c/runtime/<TGTLNAME>.uqbt.h <outdir>/uqbt.h
    ostrstream cmd;
    cmd << "cp " << prog.getProgPath() << "backend/runtime/" << TGTLNAME ".uqbt.h ";
    cmd << progOptions.outDir << "/uqbt.h";
    int ret = system(str(cmd));
    if (ret) {
        error("Could not copy uqbt.h");
        return false;
    }

    // If there is a GLOBAL ABSTRACTION section in the .pal file, then
    // add "extern char _globals[]" to uqbt.h. It should be defined in
    // either rodata.s or rwdata.s
    pair<unsigned, unsigned> gi = prog.pBF->GetGlobalPointerInfo();
    if (gi.first) {
        ostrstream cmd;
        cmd << "printf \"extern char _globals[];\\n\" >> " <<
          progOptions.outDir << "/uqbt.h";
        ret = system(str(cmd));
        if (ret) {
            error("Could not append to uqbt.h extern _globals");
            return false;
        }
    }
    // Append prototypes for the functions to uqbt.h. Open for appending.
    ofstream hf;
    hf.open((progOptions.outDir + "uqbt.h").c_str(), ios::app);
    if (!hf)
        error("Could not open uqbt.h for appending");
    else {
        hf << "\n\n/* Prototypes */\n\n";
        Proc* proc;
        PROGMAP::const_iterator pp;
        for (proc=prog.getFirstProc(pp); proc; proc = prog.getNextProc(pp)) {
            if (proc->isLib()) continue;        // Only prototype user funcs
            proc->printReturnTypeAsC(hf);                    // Return type
            hf << " " << legalCname(proc->getName()) << "("; // name (
            proc->printParamsAsC(hf);                        // params
            hf << ");\n";
        }
    }


    // Append the ints2float function. Because this depends on endianness
    // differences, we do this here rather than rely on the contents of
    // the <arch>.uqbt.h files
    hf << endl << 
       "static float64 ints2float(int32 i1, int32 i2) {\n"
       "    union {\n"
       "        float64 d;\n"
       "        struct {\n"
       "            int32 i1;\n"
       "            int32 i2;\n"
       "        } i;\n"
       "    } u;\n";
#if SRCENDIAN == TGTENDIAN
    hf <<  "    u.i.i1 = i1;\n";
    hf <<  "    u.i.i2 = i2;\n";
#else
    hf <<  "    u.i.i1 = i2;\n";
    hf <<  "    u.i.i2 = i1;\n";
#endif
    hf <<  "    return u.d;\n";
    hf <<  "}\n";

    hf.close();

#if SRCENDIAN != TGTENDIAN
    // If using the VPO backend, emit a _util.c file that contains definitions
    // of swap routines.
    if (progOptions.translateToVPO && !progOptions.noendian) {
        emitVPOUtilFile();
    }
#endif   

    // Execute the make, if requested
    if (progOptions.make) {
        chdir(progOptions.outDir.c_str());
        int ret = system("make");
        return (ret == 0);
    }
    else return true;
}

/*==============================================================================
 * FUNCTION:    OutputFile::emitVPOUtilFile
 * OVERVIEW:    Generates the _util.c source file that contains definitions
 *              of swap routines needed by code generated by VPO backends.
 *              The _util.c source file is not needed when using the low-level C
 *              backend since the generated C files make inlined calls to the
 *              swap routines declared in uqbt.h.
 * PARAMETERS:  None
 * RETURNS:     Nothing
 *============================================================================*/
void OutputFile::emitVPOUtilFile() {
    ofstream uf;
    uf.open((progOptions.outDir + "_util.c").c_str());
    if (uf == NULL) {
        error("emitVPOUtilFile: Could not open _util.c for writing");
        return;
    }
    uf << "#include \"uqbt.h\"\n";
    uf.close();
}

// This struct is needed to sort the array of procs and their names
typedef struct {
    ADDRESS nat;
    const char* pname;
} TEMP_EL;

#if __GNUC__ >= 3
namespace std {
#endif
struct less<TEMP_EL> : public binary_function<TEMP_EL, TEMP_EL, bool> {
  bool operator() (const TEMP_EL& x, const TEMP_EL& y) const
    {return x.nat < y.nat;}
};
#if __GNUC__ >= 3
}
#endif

/*==============================================================================
 * FUNCTION:    OutputFile::genRuntimeHdr
 * OVERVIEW:    Generates the _runtime.h header file. This file declares the
 *               map from source addresses (currently only procedure entry
 *               points) to target equivalents 
 * PARAMETERS:  None
 * RETURNS:     True on error
 *============================================================================*/
bool OutputFile::genRuntimeHdr()
{
    ofstream hf;
    hf.open((progOptions.outDir + "_runtime.h").c_str());
    if (!hf)
    {
        error("could not open _runtime.h for writing");
        return true;
    }

    hf << "/* Generated file */\n\n";

    // Must sort the map. Easiest way is to throw them into a set
    // Also eliminates duplicates (e.g. several symbols with value 0)
    set<TEMP_EL> s;
    PROGMAP::const_iterator it;
    Proc* proc; 
    for (proc = prog.getFirstProc(it); proc; proc = prog.getNextProc(it)) {
        TEMP_EL el;
        if (strncmp(proc->getName(), "__", 2) == 0)
            // Eliminate functions starting with double underscore, since we
            // don't speculatively decode them any more. (Else get undefined
            // references at linktime of the output code)
            continue;
        el.nat = proc->getNativeAddress(); 
        el.pname = legalCname(proc->getName());
        s.insert(el);
    }
    int numIs;
    ADDRESS* pIS;
    if (!progOptions.noLibInMap) {
        // We need to add imported library function stubs to the map
        pIS = prog.pBF->GetImportStubs(numIs);
        for (int i=0; i < numIs; i++) {
            // If the name starts with an underscore, it's likely to be a non
            // user function, and not the destination of a register call
            // Example: _umul (get not defined error at link time)
            TEMP_EL el;
            el.nat = pIS[i];
            const char* sym = prog.pBF->SymbolByAddress(pIS[i]);
            if (sym) {
                el.pname = legalCname(sym);
                if (el.pname[0] != '_')
                    s.insert(el);
            }
        }
    }

    // First prototype the proc names as voids taking no arguments
    int numMapEntries = s.size();
    set<TEMP_EL>::iterator sit;
    for (sit = s.begin(); sit != s.end(); sit++) {
        hf << "void " << sit->pname << "();\n";
    }

    // Emit the first line of declaring the map
    hf << "\nstatic MAP_EL _map[" << dec << numMapEntries << "] = {\n";

    // Emit each line in this form:
    // { 0xNATIVE, funcname},
    int n=0;
    for (sit = s.begin(); sit != s.end(); sit++) {
        hf << "\t{ 0x" << hex << sit->nat << ", " <<
            sit->pname << "}";
        if (++n != numMapEntries) hf << ",";
        hf << endl;
    }

    // Close the declaration of the struct
    hf << "};\n";

    // Delare the number of elements
    hf << "\n#define NUM_MAP " << dec << numMapEntries << endl;

    // Close the file
    hf.close();
    return false;
}


// The following code was from when objcopy was used to copy the data sections.
// For various reasons, mainly to do with segments for the moved sections, this
// didn't work except for small sparc to sparc cases. This code is left here in
// case it it ever needed to invoke objcopy again.
#if 0
    // Now replace each (dummy) data section with the real data section. We
    // use the GNU objcopy (a binutil) for this:
    //  -R means remove this section (from the input file, if it exists)
    //  --add-section means add this section (to the output file, using this
    //    datafile)
    //  --adjust-section-vma means set the section's virtual memory address to
    //    the given value
    //  --set-section-flags means to set the flags as appropriate. All must be
    //    allocated and loaded, and some must be readonly.
    
    DATALIST::iterator ii;
    for (ii = dataInfo.begin(); ii != dataInfo.end(); ii++)
    {
        // objcopy -R <section> --add-section=section=data<section>
        //  --adjust-section-vma=<section>=vma
        //  --set-section-flags=<section>=flag1,flag2 <prog>.uqbt
        ostrstream cmd;
        const string& ProgPath = prog.GetProgPath();
        cmd << ProgPath << "objcopy -R " << (*ii).section + " --add-section=" +
            (*ii).section << "=data" << (*ii).section <<
            " --adjust-section-vma=" << (*ii).section << "=0x" << hex <<
            (*ii).address << " --set-section-flags=" << (*ii).section <<
            "=alloc,load,data";
        if ((*ii).readonly)
            cmd << ",readonly";
        cmd << " " << prog.GetName() << ".uqbt";
        system(str(cmd));
    }
#endif