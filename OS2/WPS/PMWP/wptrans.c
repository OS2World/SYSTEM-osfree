/*
 *  This file was generated by the SOM Compiler and Emitter Framework.
 *  Generated using template emitter:
 *      SOM Emitter emitctm: 2.23.1.9
 */

#ifndef SOM_Module_wptrans_Source
#define SOM_Module_wptrans_Source
#endif
#define WPTransient_Class_Source
#define M_WPTransient_Class_Source

#define INCL_PMWP
#include <os2.h>

#include "wptrans.ih"


SOM_Scope HOBJECT  SOMLINK trn_wpQueryHandle(WPTransient *somSelf)
{
    /* WPTransientData *somThis = WPTransientGetData(somSelf); */
    WPTransientMethodDebug("WPTransient","trn_wpQueryHandle");

    return (WPTransient_parent_WPObject_wpQueryHandle(somSelf));
}

SOM_Scope WPObject*  SOMLINK trn_wpCopyObject(WPTransient *somSelf, 
                                              WPFolder* Folder, 
                                              BOOL fLock)
{
    /* WPTransientData *somThis = WPTransientGetData(somSelf); */
    WPTransientMethodDebug("WPTransient","trn_wpCopyObject");

    return (WPTransient_parent_WPObject_wpCopyObject(somSelf, 
                                                     Folder, 
                                                     fLock));
}

SOM_Scope void SOMLINK trn_somDefaultInit(WPTransient *somSelf, 
                                          somInitCtrl* ctrl)
{
    WPTransientData *somThis; /* set in BeginInitializer */
    somInitCtrl globalCtrl;
    somBooleanVector myMask;
    WPTransientMethodDebug("WPTransient","somDefaultInit");
    WPTransient_BeginInitializer_somDefaultInit;

    WPTransient_Init_WPObject_somDefaultInit(somSelf, ctrl);

    /*
     * local WPTransient initialization code added by programmer
     */
}


SOM_Scope void SOMLINK trn_somDestruct(WPTransient *somSelf, 
                                       octet doFree, somDestructCtrl* ctrl)
{
    WPTransientData *somThis; /* set in BeginDestructor */
    somDestructCtrl globalCtrl;
    somBooleanVector myMask;
    WPTransientMethodDebug("WPTransient","trn_somDestruct");
    WPTransient_BeginDestructor;

    /*
     * local WPTransient deinitialization code added by programmer
     */

    WPTransient_EndDestructor;
}


SOM_Scope HPOINTER  SOMLINK trn_wpQueryIcon(WPTransient *somSelf)
{
    /* WPTransientData *somThis = WPTransientGetData(somSelf); */
    WPTransientMethodDebug("WPTransient","trn_wpQueryIcon");

    return (WPTransient_parent_WPObject_wpQueryIcon(somSelf));
}

SOM_Scope BOOL  SOMLINK trn_wpSaveImmediate(WPTransient *somSelf)
{
    /* WPTransientData *somThis = WPTransientGetData(somSelf); */
    WPTransientMethodDebug("WPTransient","trn_wpSaveImmediate");

    return (WPTransient_parent_WPObject_wpSaveImmediate(somSelf));
}

SOM_Scope BOOL  SOMLINK trn_wpIdentify(WPTransient *somSelf, 
                                       PSZ pszIdentity)
{
    /* WPTransientData *somThis = WPTransientGetData(somSelf); */
    WPTransientMethodDebug("WPTransient","trn_wpIdentify");

    return (WPTransient_parent_WPObject_wpIdentify(somSelf, pszIdentity));
}

SOM_Scope BOOL  SOMLINK trn_wpDestroyObject(WPTransient *somSelf)
{
    /* WPTransientData *somThis = WPTransientGetData(somSelf); */
    WPTransientMethodDebug("WPTransient","trn_wpDestroyObject");

    return (WPTransient_parent_WPObject_wpDestroyObject(somSelf));
}


SOM_Scope ULONG  SOMLINK trnM_wpclsLockTransientList(M_WPTransient *somSelf)
{
    /* M_WPTransientData *somThis = M_WPTransientGetData(somSelf); */
    M_WPTransientMethodDebug("M_WPTransient","trnM_wpclsLockTransientList");

    /* Return statement to be customized: */
    return;
}

SOM_Scope ULONG  SOMLINK trnM_wpclsUnlockTransientList(M_WPTransient *somSelf)
{
    /* M_WPTransientData *somThis = M_WPTransientGetData(somSelf); */
    M_WPTransientMethodDebug("M_WPTransient","trnM_wpclsUnlockTransientList");

    /* Return statement to be customized: */
    return;
}

SOM_Scope PSZ  SOMLINK trnM_wpclsQueryTitle(M_WPTransient *somSelf)
{
    M_WPTransientData *somThis = M_WPTransientGetData(somSelf);
    M_WPTransientMethodDebug("M_WPTransient","trnM_wpclsQueryTitle");

    return (M_WPTransient_parent_M_WPObject_wpclsQueryTitle(somSelf));
}

SOM_Scope ULONG  SOMLINK trnM_wpclsQueryStyle(M_WPTransient *somSelf)
{
    M_WPTransientData *somThis = M_WPTransientGetData(somSelf);
    M_WPTransientMethodDebug("M_WPTransient","trnM_wpclsQueryStyle");

    return (M_WPTransient_parent_M_WPObject_wpclsQueryStyle(somSelf));
}

SOM_Scope WPObject*  SOMLINK trnM_wpclsObjectFromHandle(M_WPTransient *somSelf, 
                                                        HOBJECT hObject)
{
    M_WPTransientData *somThis = M_WPTransientGetData(somSelf);
    M_WPTransientMethodDebug("M_WPTransient","trnM_wpclsObjectFromHandle");

    return (M_WPTransient_parent_M_WPObject_wpclsObjectFromHandle(somSelf, 
                                                                  hObject));
}

