
/*
 *@@sourcefile xwpnetsrv.c:
 *      This file contains SOM code for the following XWorkplace classes:
 *
 *      --  XWPNetServer: a subclass of WPTransient, which can only
 *          appear in a XWPNetwork folder.
 *
 *@@somclass XWPNetServer xsrv_
 *@@somclass M_XWPNetServer xsrvM_
 */

/*
 *      Copyright (C) 2001-2003 Ulrich M�ller.
 *
 *      This file is part of the XWorkplace source package.
 *      XWorkplace is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published
 *      by the Free Software Foundation, in version 2 as it comes in the
 *      "COPYING" file of the XWorkplace main distribution.
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 */

/*
 *  This file was generated by the SOM Compiler and Emitter Framework.
 *  Generated using:
 *      SOM Emitter emitctm: 2.41
 */

#ifndef SOM_Module_xwpnetsrv_Source
#define SOM_Module_xwpnetsrv_Source
#endif
#define XWPNetServer_Class_Source
#define M_XWPNetServer_Class_Source

#pragma strings(readonly)

/*
 *  Suggested #include order:
 *  1)  os2.h
 *  2)  C library headers
 *  3)  setup.h (code generation and debugging options)
 *  4)  headers in helpers\
 *  5)  at least one SOM implementation header (*.ih)
 *  6)  dlgids.h, headers in shared\ (as needed)
 *  7)  headers in implementation dirs (e.g. filesys\, as needed)
 *  8)  #pragma hdrstop and then more SOM headers which crash with precompiled headers
 */

#define INCL_DOSPROCESS
#define INCL_DOSSEMAPHORES
#define INCL_DOSEXCEPTIONS
#define INCL_DOSMODULEMGR
#define INCL_DOSERRORS
#include <os2.h>

// #include <netcons.h>

// C library headers
#include <stdio.h>              // needed for except.h
#include <setjmp.h>             // needed for except.h
#include <assert.h>             // needed for except.h

// generic headers
#include "setup.h"                      // code generation and debugging options

// headers in /helpers
#include "helpers\lan.h"
#include "helpers\stringh.h"            // string helper routines

// SOM headers which don't crash with prec. header files
#include "xwpnetsrv.ih"

// XWorkplace implementation headers
#include "shared\common.h"              // the majestic XWorkplace include file
#include "shared\helppanels.h"          // all XWorkplace help panel IDs
#include "shared\kernel.h"              // XWorkplace Kernel

// other SOM headers
#pragma hdrstop

/* ******************************************************************
 *
 *   XWPNetServer instance methods
 *
 ********************************************************************/

/*
 *@@ wpInitData:
 *
 */

SOM_Scope void  SOMLINK xsrv_wpInitData(XWPNetServer *somSelf)
{
    XWPNetServerData *somThis = XWPNetServerGetData(somSelf);
    XWPNetServerMethodDebug("XWPNetServer","xsrv_wpInitData");

    XWPNetServer_parent_WPTransient_wpInitData(somSelf);

    _pszServerName = NULL;
}

/*
 *@@ wpUnInitData:
 *      this WPObject instance method is called when the object
 *      is destroyed as a SOM object, either because it's being
 *      made dormant (via wpMakeDormant) or being deleted for
 *      good (via wpFree). All allocated in-memory resources
 *      should be freed here, but to destroy the physical
 *      representation of the object, override wpDestroyObject
 *      instead.
 *
 *      The parent method must always be called last.
 */

SOM_Scope void  SOMLINK xsrv_wpUnInitData(XWPNetServer *somSelf)
{
    XWPNetServerData *somThis = XWPNetServerGetData(somSelf);
    XWPNetServerMethodDebug("XWPNetServer","xsrv_wpUnInitData");

    strhStore(&_pszServerName, NULL, NULL);

    XWPNetServer_parent_WPTransient_wpUnInitData(somSelf);
}

/*
 *@@ wpSetupOnce:
 *
 */

SOM_Scope BOOL  SOMLINK xsrv_wpSetupOnce(XWPNetServer *somSelf,
                                         PSZ pszSetupString)
{
    BOOL brc;

    XWPNetServerData *somThis = XWPNetServerGetData(somSelf);
    XWPNetServerMethodDebug("XWPNetServer","xsrv_wpSetupOnce");

    if (brc = XWPNetServer_parent_WPTransient_wpSetupOnce(somSelf,
                                                          pszSetupString))
    {
        CHAR szServerName[CNLEN + 1];
        ULONG cb = sizeof(szServerName);
        if (_wpScanSetupString(somSelf,
                               pszSetupString,
                               "SERVERNAME",
                               szServerName,
                               &cb))
            strhStore(&_pszServerName, szServerName, NULL);
        else
            // if this string is not present, FAIL (stop object creation)
            brc = FALSE;
    }

    return brc;
}

/*
 *@@ wpQueryDetailsData:
 *
 */

SOM_Scope ULONG  SOMLINK xsrv_wpQueryDetailsData(XWPNetServer *somSelf,
                                                 PVOID* ppDetailsData,
                                                 PULONG pcp)
{
    XWPNetServerData *somThis = XWPNetServerGetData(somSelf);
    XWPNetServerMethodDebug("XWPNetServer","xsrv_wpQueryDetailsData");

    return (XWPNetServer_parent_WPTransient_wpQueryDetailsData(somSelf,
                                                               ppDetailsData,
                                                               pcp));
}

/*
 *@@ wpFilterPopupMenu:
 *
 */

SOM_Scope ULONG  SOMLINK xsrv_wpFilterPopupMenu(XWPNetServer *somSelf,
                                                ULONG ulFlags,
                                                HWND hwndCnr,
                                                BOOL fMultiSelect)
{
    XWPNetServerData *somThis = XWPNetServerGetData(somSelf);
    XWPNetServerMethodDebug("XWPNetServer","xsrv_wpFilterPopupMenu");

    return (XWPNetServer_parent_WPTransient_wpFilterPopupMenu(somSelf,
                                                              ulFlags,
                                                              hwndCnr,
                                                              fMultiSelect)
             & ~(
                    CTXT_NEW
                )
             |  (
                      CTXT_OPEN
                    | CTXT_ICON
                    | CTXT_DETAILS
                    | CTXT_TREE
                    | CTXT_SETTINGS
                ));
}

/*
 *@@ wpModifyPopupMenu:
 *
 */

SOM_Scope BOOL  SOMLINK xsrv_wpModifyPopupMenu(XWPNetServer *somSelf,
                                               HWND hwndMenu,
                                               HWND hwndCnr,
                                               ULONG iPosition)
{
    XWPNetServerData *somThis = XWPNetServerGetData(somSelf);
    XWPNetServerMethodDebug("XWPNetServer","xsrv_wpModifyPopupMenu");

    return (XWPNetServer_parent_WPTransient_wpModifyPopupMenu(somSelf,
                                                              hwndMenu,
                                                              hwndCnr,
                                                              iPosition));
}

/*
 *@@ wpMenuItemSelected:
 *
 */

SOM_Scope BOOL  SOMLINK xsrv_wpMenuItemSelected(XWPNetServer *somSelf,
                                                HWND hwndFrame,
                                                ULONG ulMenuId)
{
    XWPNetServerData *somThis = XWPNetServerGetData(somSelf);
    XWPNetServerMethodDebug("XWPNetServer","xsrv_wpMenuItemSelected");

    return (XWPNetServer_parent_WPTransient_wpMenuItemSelected(somSelf,
                                                               hwndFrame,
                                                               ulMenuId));
}

/*
 *@@ wpMenuItemHelpSelected:
 *
 */

SOM_Scope BOOL  SOMLINK xsrv_wpMenuItemHelpSelected(XWPNetServer *somSelf,
                                                    ULONG MenuId)
{
    XWPNetServerData *somThis = XWPNetServerGetData(somSelf);
    XWPNetServerMethodDebug("XWPNetServer","xsrv_wpMenuItemHelpSelected");

    return (XWPNetServer_parent_WPTransient_wpMenuItemHelpSelected(somSelf,
                                                                   MenuId));
}

/*
 *@@ wpMoveObject:
 *
 */

SOM_Scope BOOL  SOMLINK xsrv_wpMoveObject(XWPNetServer *somSelf,
                                          WPFolder* Folder)
{
    XWPNetServerData *somThis = XWPNetServerGetData(somSelf);
    XWPNetServerMethodDebug("XWPNetServer","xsrv_wpMoveObject");

    return (XWPNetServer_parent_WPTransient_wpMoveObject(somSelf,
                                                         Folder));
}

/*
 *@@ wpOpen:
 *
 */

SOM_Scope HWND  SOMLINK xsrv_wpOpen(XWPNetServer *somSelf, HWND hwndCnr,
                                    ULONG ulView, ULONG param)
{
    XWPNetServerData *somThis = XWPNetServerGetData(somSelf);
    XWPNetServerMethodDebug("XWPNetServer","xsrv_wpOpen");

    return (XWPNetServer_parent_WPTransient_wpOpen(somSelf, hwndCnr,
                                                   ulView, param));
}

/*
 *@@ wpDragOver:
 *
 */

SOM_Scope MRESULT  SOMLINK xsrv_wpDragOver(XWPNetServer *somSelf,
                                           HWND hwndCnr, PDRAGINFO pdrgInfo)
{
    XWPNetServerData *somThis = XWPNetServerGetData(somSelf);
    XWPNetServerMethodDebug("XWPNetServer","xsrv_wpDragOver");

    return (MRESULT)DOR_NEVERDROP;   // object does not accept d'n'd
}

/*
 *@@ wpDrop:
 *
 */

SOM_Scope MRESULT  SOMLINK xsrv_wpDrop(XWPNetServer *somSelf,
                                       HWND hwndCnr, PDRAGINFO pdrgInfo,
                                       PDRAGITEM pdrgItem)
{
    XWPNetServerData *somThis = XWPNetServerGetData(somSelf);
    XWPNetServerMethodDebug("XWPNetServer","xsrv_wpDrop");

    return (MRESULT)RC_DROP_ERROR;
}


/* ******************************************************************
 *
 *   XWPNetServer class methods
 *
 ********************************************************************/

/*
 *@@ wpclsInitData:
 *      this M_WPObject class method gets called when a class
 *      is loaded by the WPS (probably from within a
 *      somFindClass call) and allows the class to initialize
 *      itself.
 */

SOM_Scope void  SOMLINK xsrvM_wpclsInitData(M_XWPNetServer *somSelf)
{
    /* M_XWPNetServerData *somThis = M_XWPNetServerGetData(somSelf); */
    M_XWPNetServerMethodDebug("M_XWPNetServer","xsrvM_wpclsInitData");

    M_XWPNetServer_parent_M_WPTransient_wpclsInitData(somSelf);

    krnClassInitialized(G_pcszXWPNetServer);
}

/*
 *@@ wpclsCreateDefaultTemplates:
 *
 */

SOM_Scope BOOL  SOMLINK xsrvM_wpclsCreateDefaultTemplates(M_XWPNetServer *somSelf,
                                                          WPObject* Folder)
{
    /* M_XWPNetServerData *somThis = M_XWPNetServerGetData(somSelf); */
    M_XWPNetServerMethodDebug("M_XWPNetServer","xsrvM_wpclsCreateDefaultTemplates");

    return TRUE;
    // means that the Templates folder should _not_ create templates
    // by itself; we pretend that we've done this
}

/*
 *@@ wpclsQueryDetailsInfo:
 *
 */

SOM_Scope ULONG  SOMLINK xsrvM_wpclsQueryDetailsInfo(M_XWPNetServer *somSelf,
                                                     PCLASSFIELDINFO* ppClassFieldInfo,
                                                     PULONG pSize)
{
    /* M_XWPNetServerData *somThis = M_XWPNetServerGetData(somSelf); */
    M_XWPNetServerMethodDebug("M_XWPNetServer","xsrvM_wpclsQueryDetailsInfo");

    return (M_XWPNetServer_parent_M_WPTransient_wpclsQueryDetailsInfo(somSelf,
                                                                      ppClassFieldInfo,
                                                                      pSize));
}

/*
 *@@ wpclsQueryTitle:
 *
 */

SOM_Scope PSZ  SOMLINK xsrvM_wpclsQueryTitle(M_XWPNetServer *somSelf)
{
    /* M_XWPNetServerData *somThis = M_XWPNetServerGetData(somSelf); */
    M_XWPNetServerMethodDebug("M_XWPNetServer","xsrvM_wpclsQueryTitle");

    return "Network server";  // @@todo localize
}

/*
 *@@ wpclsQueryStyle:
 *
 */

SOM_Scope ULONG  SOMLINK xsrvM_wpclsQueryStyle(M_XWPNetServer *somSelf)
{
    /* M_XWPNetServerData *somThis = M_XWPNetServerGetData(somSelf); */
    M_XWPNetServerMethodDebug("M_XWPNetServer","xsrvM_wpclsQueryStyle");

    return (CLSSTYLE_NEVERTEMPLATE
                | CLSSTYLE_NEVERCOPY
                | CLSSTYLE_NEVERMOVE
                | CLSSTYLE_NEVERLINK
                | CLSSTYLE_NEVERDELETE  // we have a "destroy" menu item already
                | CLSSTYLE_NEVERDROPON
                | CLSSTYLE_NEVERPRINT
                | CLSSTYLE_NEVERRENAME
                | CLSSTYLE_NEVERSETTINGS);
}

/*
 *@@ wpclsQueryDefaultHelp:
 *      this M_WPObject class method returns the default help
 *      panel for objects of this class. This gets called
 *      from WPObject::wpQueryDefaultHelp if no instance
 *      help settings (HELPLIBRARY, HELPPANEL) have been
 *      set for an individual object. It is thus recommended
 *      to override this method instead of the instance
 *      method to change the default help panel for a class
 *      in order not to break instance help settings (fixed
 *      with 0.9.20).
 *
 *@@added V0.9.20 (2002-07-12) [umoeller]
 */

SOM_Scope BOOL  SOMLINK xsrvM_wpclsQueryDefaultHelp(M_XWPNetServer *somSelf,
                                                    PULONG pHelpPanelId,
                                                    PSZ pszHelpLibrary)
{
    /* M_XWPNetServerData *somThis = M_XWPNetServerGetData(somSelf); */
    M_XWPNetServerMethodDebug("M_XWPNetServer","xsrvM_wpclsQueryDefaultHelp");

    strcpy(pszHelpLibrary, cmnQueryHelpLibrary());
    *pHelpPanelId = ID_XSH_XWPNETSERVER_MAIN;
    return TRUE;
}

/*
 *@@ wpclsQueryIconData:
 *      this M_WPObject class method must return information
 *      about how to build the default icon for objects
 *      of a class. This gets called from various other
 *      methods whenever a class default icon is needed;
 *      most importantly, M_WPObject::wpclsQueryIcon
 *      calls this to build a class default icon, which
 *      is then cached in the class's instance data.
 *      If a subclass wants to change a class default icon,
 *      it should always override _this_ method instead of
 *      wpclsQueryIcon.
 *
 *      Note that the default WPS implementation does not
 *      allow for specifying the ICON_FILE format here,
 *      which is why we have overridden
 *      M_XFldObject::wpclsQueryIcon too. This allows us
 *      to return icon _files_ for theming too. For details
 *      about the WPS's crappy icon management, refer to
 *      src\filesys\icons.c.
 *
 */

SOM_Scope ULONG  SOMLINK xsrvM_wpclsQueryIconData(M_XWPNetServer *somSelf,
                                                  PICONINFO pIconInfo)
{
    /* M_XWPNetServerData *somThis = M_XWPNetServerGetData(somSelf); */
    M_XWPNetServerMethodDebug("M_XWPNetServer","xsrvM_wpclsQueryIconData");

    if (pIconInfo)
    {
        if (!DosQueryModuleHandle("PMWP",
                                  &pIconInfo->hmod))
        {
             pIconInfo->fFormat = ICON_RESOURCE;
             pIconInfo->resid   = 63;   // active server icon
                                  // 67;   // inactive server icon
             // pIconInfo->hmod    = cmnQueryMainResModuleHandle();
        }
        else
            return (M_XWPNetServer_parent_M_WPTransient_wpclsQueryIconData(somSelf,
                                                                           pIconInfo));
    }

    return sizeof(ICONINFO);
}

