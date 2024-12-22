#ifndef MINISTD_H
#define MINISTD_H

#include <Ldr.h>
#include <common/phnt.h>


INT MemCompare( _In_ PVOID m1, _In_ PVOID m2, _In_ INT len );

PVOID CopyMemoryEx( _Inout_ PVOID Destination, _In_ PVOID Source, _In_ SIZE_T Length );
VOID ZeroMemoryEx( _Inout_ PVOID Destination, _In_ SIZE_T Size );

SIZE_T StrLenA( _In_ LPCSTR String );
SIZE_T StrLenW( _In_ LPCWSTR String );
INT StringCompareA( _In_ LPCSTR String1, _In_ LPCSTR String2 );
INT StringCompareW( _In_ LPCWSTR String1, _In_ LPCWSTR String2 );
BOOL IsStringEqualA( _In_ LPCSTR Str1, _In_ LPCSTR Str2 );
BOOL IsStringEqualW( _In_ LPCWSTR Str1, _In_ LPCWSTR Str2 );

SIZE_T CharStringToWCharString( _Inout_ PWCHAR Destination, _In_ PCHAR Source, _In_ SIZE_T MaximumAllowed );
SIZE_T WCharStringToCharString( _Inout_ PCHAR Destination, _In_ PWCHAR Source, _In_ SIZE_T MaximumAllowed );

BYTE HideChar( BYTE C );

#endif // MINISTD_H
