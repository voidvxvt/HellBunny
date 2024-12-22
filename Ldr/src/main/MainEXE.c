#include <Ldr.h>
#include <Config.h>
#include <core/SysNative.h>
#include <common/Debug.h>


#if ( defined( EXE ) && ( EXE == TRUE ) )

#pragma comment( linker, "/ENTRY:WinMain" )
INT WINAPI 
WinMain( 
    HINSTANCE hInstance, 
    HINSTANCE hPrevInstance, 
    LPSTR     lpCmdLine, 
    INT       nShowCmd
) {
    DBGINIT()
    PRINTF( "WinMain: hInstance:[%p] hPrevInstance:[%p] lpCmdLine:[%s] nShowCmd:[%d]\n", hInstance, hPrevInstance, lpCmdLine, nShowCmd );
    
    LdrMain();

    DBGEXIT()
    SysNtTerminateProcess( NtCurrentProcess(), STATUS_SUCCESS );
    return 0;
}

#endif // EXE == TRUE
