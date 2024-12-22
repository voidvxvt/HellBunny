#pragma once
#ifndef DEBUG_H
#define DEBUG_H

#include <common/phnt.h>
#include <common/Macros.h>
#include <stdio.h>

#if ( defined( _DEBUG ) && _DEBUG == TRUE )

typedef struct _DbgCtx {

    WIN_API( GetConsoleWindow )
    WIN_API( AllocConsole )
    WIN_API( SetConsoleTitleA )
    WIN_API( GetStdHandle )
    WIN_API( WriteConsoleA )
    WIN_API( ReadConsoleA )
    WIN_API( FreeConsole )
    WIN_API( vsnprintf )

    WIN_API( GetThreadId )

    HANDLE ConsoleOutputHandle;
    HANDLE ConsoleInputHandle;
} DBGCTX, *PDBGCTX;

PDBGCTX Dbg;

BOOL g_DbgCtx;

BOOL g_Console;

VOID 
DbgCtxInit( 
    VOID
);

VOID
DbgConsoleInit(
    VOID
);

VOID 
DbgPrintF(
    const char* const _Format, 
    ...
);

VOID DbgGetChar(
    VOID
);

#define DBGINIT()                                                       \
{                                                                       \
    DbgCtxInit();                                                       \
    DbgConsoleInit();                                                   \
    DbgPrintF( "[!] ======== Started Debug Console ======== [!]\r\n" ); \
}

#define DBGEXIT()                                                       \
{                                                                       \
    DbgPrintF( "[!] ======== Stopped Debug Console ======== [!]\r\n" ); \
    DbgPrintF( "[!] Press <ENTER> to close Debug Console #>" );         \
    DbgGetChar();                                                       \
    Dbg->FreeConsole();                                                 \
}

#if ( defined( DLL ) && ( DLL == TRUE ) ) || \
    ( defined( XLL ) && ( XLL == TRUE ) )
    #define DBGDLLINIT() { DBGINIT() }
    #define DBGDLLEXIT() { DBGEXIT() }
#else 
    #define DBGDLLINIT() { ; }
    #define DBGDLLEXIT() { ; }
#endif

#define PUTS( s )           { DbgPrintF( "[DEBUG::%s::%d] " s "\r\n", __FUNCTION__, __LINE__ ); }
#define PRINTF( f, ... )    { DbgPrintF( "[DEBUG::%s::%d] " f "\r\n", __FUNCTION__, __LINE__, __VA_ARGS__ ); }
#define GETCHAR()           { DbgGetChar(); }
#define PRINT_HEX( b, l )                                           \
    DbgPrintF( #b ": [%zu] [ ", l );                                \
    for ( SIZE_T i = 0 ; i < l ; i++ )                              \
    {                                                               \
        DbgPrintF( "%02x ", ( ( PBYTE ) b ) [ i ] );                \
    }                                                               \
    DbgPrintF( "]\r\n" );


#else // ( defined( _DEBUG ) && _DEBUG == TRUE )


#define DBGINIT() { ; }
#define DBGEXIT() { ; }
#define DBGDLLINIT() { ; }
#define DBGDLLEXIT() { ; }

#define PUTS( s )           { ; }
#define PRINTF( f, ... )    { ; }
#define GETCHAR( )          { ; }

#define PRINT_HEX( b, l ) {}


#endif // ( defined( _DEBUG ) && _DEBUG == TRUE )


#endif//DEBUG_H