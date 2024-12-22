#include <common/Debug.h>
#include <common/phnt.h>


#if ( defined( _DEBUG ) && _DEBUG == TRUE )
VOID
DbgCtxInit( 
    VOID
) {
    if ( g_DbgCtx ) 
        return;

    static DBGCTX DbgCtx = { 0 };
    Dbg = & DbgCtx;

    g_DbgCtx = TRUE;

    PVOID Kernel32 = NULL;
    PVOID MsvCrt   = NULL;

    if ( Kernel32 = LoadLibraryA( "KERNEL32.DLL" ) ) 
    {
        Dbg->GetConsoleWindow = GetProcAddress( Kernel32, "GetConsoleWindow" );
        Dbg->AllocConsole     = GetProcAddress( Kernel32, "AllocConsole"     );
        Dbg->SetConsoleTitleA = GetProcAddress( Kernel32, "SetConsoleTitleA" );
        Dbg->GetStdHandle     = GetProcAddress( Kernel32, "GetStdHandle"     );
        Dbg->WriteConsoleA    = GetProcAddress( Kernel32, "WriteConsoleA"    );
        Dbg->ReadConsoleA     = GetProcAddress( Kernel32, "ReadConsoleA"     );
        Dbg->FreeConsole      = GetProcAddress( Kernel32, "FreeConsole"      );

        Dbg->GetThreadId      = GetProcAddress( Kernel32, "GetThreadId"      );
    }

    if ( MsvCrt = LoadLibraryA( "MsvCrt.DLL" ) ) 
    {
        Dbg->vsnprintf = GetProcAddress( MsvCrt, "vsnprintf" );
    }

    return;
}

VOID
DbgConsoleInit(
    VOID
) {
    if ( g_Console ) 
        return;

    if ( ! Dbg->GetConsoleWindow() && Dbg->AllocConsole() ) {
        g_Console = TRUE;
    }

    Dbg->SetConsoleTitleA( "DEBUG CONSOLE" );

    Dbg->ConsoleOutputHandle = Dbg->GetStdHandle( STD_OUTPUT_HANDLE );
    if ( Dbg->ConsoleOutputHandle == INVALID_HANDLE_VALUE ) {
        return;
    }

    Dbg->ConsoleInputHandle = Dbg->GetStdHandle( STD_INPUT_HANDLE );
    if ( Dbg->ConsoleInputHandle != INVALID_HANDLE_VALUE ) {
        return;
    }

    return;
}

#define BUF_MAX ( MAX_PATH * 2 )
VOID 
DbgPrintF(
    const char* const _Format, 
    ...
) {
    PSTR    cBuffer[ BUF_MAX ] = { 0 };
    va_list va_args            = { 0 };
    INT     iLength            = 0;
    DWORD   BytesWritten       = 0;

    va_start( va_args, _Format );
    iLength = Dbg->vsnprintf( cBuffer, BUF_MAX, _Format, va_args );
    va_end( va_args );

    Dbg->WriteConsoleA( Dbg->ConsoleOutputHandle, cBuffer, iLength, &BytesWritten, NULL );

    MemZero( cBuffer, BUF_MAX );

    return;
}

VOID DbgGetChar(
    VOID
) {
    BYTE  buf[ 1 ]  = { 0 };
    DWORD BytesRead = 0;
    
    Dbg->ReadConsoleA( Dbg->ConsoleInputHandle, &buf, 1, &BytesRead, NULL );

    return;
}

#endif // _DEBUG == TRUE
