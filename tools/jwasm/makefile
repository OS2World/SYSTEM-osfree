# $Id: makefile,v 1.1 2004/08/16 06:27:05 prokushev Exp $

PROJ = jwasm
TRGT = $(PROJ).exe
DESC = Enhanced OW assembler
#DIRS = port
srcfiles = $(p)apiemu$(e) $(p)assemble$(e) $(p)assume$(e) $(p)atofloat$(e) $(p)backptch$(e) &
  $(p)bin$(e) $(p)branch$(e) $(p)cmdline$(e) $(p)codegen$(e) $(p)coff$(e) $(p)condasm$(e)   &
  $(p)context$(e) $(p)cpumodel$(e) $(p)data$(e) $(p)dbgcv$(e) $(p)directiv$(e) $(p)elf$(e)  &
  $(p)end$(e) $(p)equate$(e) $(p)errmsg$(e) $(p)expans$(e) $(p)expreval$(e) $(p)extern$(e)  &
  $(p)fastpass$(e) $(p)fixup$(e) $(p)fpfixup$(e) $(p)hll$(e) $(p)input$(e) $(p)invoke$(e)   &
  $(p)label$(e) $(p)linnum$(e) $(p)listing$(e) $(p)loop$(e) $(p)lqueue$(e) $(p)macro$(e)    &
  $(p)mangle$(e) $(p)memalloc$(e) $(p)msgtext$(e) $(p)omf$(e) $(p)omffixup$(e) $(p)omfint$(e) &
  $(p)option$(e) $(p)parser$(e) $(p)posndir$(e) $(p)preproc$(e) $(p)proc$(e) $(p)queue$(e)  &
  $(p)reswords$(e) $(p)safeseh$(e) $(p)segment$(e) $(p)simsegm$(e) $(p)string$(e)           &
  $(p)symbols$(e) $(p)tbyte$(e) $(p)tokenize$(e) $(p)types$(e) $(p)main$(e)

!ifeq %OS OS2
defs = $(%WATCOM)$(SEP)h$(SEP)os2
!endif
!ifeq %OS WIN32
defs = $(%WATCOM)$(SEP)h$(SEP)nt
!endif
!ifeq %OS DOS
defs = $(%WATCOM)$(SEP)h$(SEP)dos
!endif
!ifeq %OS LINUX
defs = $(%WATCOM)$(SEP)lh
!endif

!include $(%ROOT)tools/mk/tools.mk
!include $(MYDIR)jwasm.mk

ADD_COPT = -dNDEBUG -i=$(PORT_BASE)src$(SEP)H -i=$(defs)
ADD_LINKOPT = lib clib3r
STACKSIZE = 0x20000
#UNI2H = 1
C = r

.c: $(PORT_BASE)src

.h: $(PORT_BASE)src$(SEP)H

.$(O): $(PATH)

$(PATH)msgtext.$(O): $(PORT_BASE)src$(SEP)msgtext.c &
                     $(PORT_BASE)src$(SEP)H$(SEP)msgdef.h &
                     $(PORT_BASE)src$(SEP)H$(SEP)globals.h

$(PATH)reswords.$(O): $(PORT_BASE)src$(SEP)reswords.c &
                      $(PORT_BASE)src$(SEP)H$(SEP)instruct.h &
                      $(PORT_BASE)src$(SEP)H$(SEP)special.h  &
                      $(PORT_BASE)src$(SEP)H$(SEP)directve.h
