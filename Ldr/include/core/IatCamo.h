#pragma once
#ifndef IAT_CAMO_H
#define IAT_CAMO_H

#include <common/phnt.h>


INT 
RandomCompileTimeSeed( 
    VOID 
) {
    return '0' * -40261 +
        __TIME__[ 7 ] * 1 +
        __TIME__[ 6 ] * 10 +
        __TIME__[ 4 ] * 60 +
        __TIME__[ 3 ] * 600 +
        __TIME__[ 1 ] * 3600 +
        __TIME__[ 0 ] * 36000;
}

PVOID 
Helper( 
    PVOID* AddrPtr
) {
    PVOID Addr = HeapAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY, 0xFF );
    if ( ! Addr ) {
        return NULL;
    }

    // setting the first 4 bytes in AddrPtr to be equal to a ranndom number ( less than 255 )
    *( PINT ) Addr = RandomCompileTimeSeed() % 0xFF;

    // saving the base address by pointer
    *AddrPtr = Addr;

    return Addr;
}

// function that imports WinAPIs but never uses them
VOID
IatCamouflage( 
    VOID
) {
    PVOID Addr = NULL;
    PINT  A    = ( PINT )Helper( & Addr );
    
    unsigned __int64 x = NULL;

    // impossible conditional statement that will never run
    if ( *A > 350 )
    {
        // random whistelisted WinAPIs
        x = MessageBoxA( NULL, NULL, NULL, NULL );
        x = GetLastError( );
        x = CreateWindowExW( NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL );
        x = IsDialogMessageW( NULL, NULL );
        x = IsWindow( NULL );
        x = IsWindowEnabled( NULL );
        x = IsWindowVisible( NULL );
        x = ToUnicodeEx( NULL, NULL, NULL, NULL, NULL, NULL, NULL );
        x = UpdateLayeredWindow( NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL );
        x = RegisterWindowMessageW( NULL );
        x = SetWindowDisplayAffinity( NULL, NULL );
        x = SetWindowTextW( NULL, NULL );
        x = LoadIconW( NULL, NULL );
        x = LoadCursorW( NULL, NULL );
        x = LoadStringW( NULL, NULL, NULL, NULL );
        x = LoadImageW( NULL, NULL, NULL, NULL, NULL, NULL );
        x = GetWindow( NULL, NULL );
        x = GetWindowInfo( NULL, NULL );
        x = GetWindowContextHelpId( NULL );
        x = GetWindowRect( NULL, NULL );
        x = GetWindowRgn( NULL, NULL );
        x = GetWindowPlacement( NULL, NULL );
        x = GetSysColor( NULL );
        x = GetSysColorBrush( NULL );
        x = GetSystemMenu( NULL, NULL );
        x = GetMenuItemCount( NULL );
        x = GetMenuItemInfoW( NULL, NULL, NULL, NULL );
        x = GetMenuBarInfo( NULL, NULL, NULL, NULL );

    }

    HeapFree( GetProcessHeap(), 0, Addr );
}

#endif // IAT_CAMO_H