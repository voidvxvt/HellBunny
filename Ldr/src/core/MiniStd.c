#include <core/MiniStd.h>

// most functions are from https://github.com/vxunderground/VX-API

// https://github.com/xrombar/flower/blob/06ba9f1eca599c1ea157e81868ab61ab57d35c8a/src/Flower.c#L1181
INT
MemCompare(
    _In_ PVOID m1,
    _In_ PVOID m2,
    _In_ INT   len
) {
    PUCHAR p = m1;
    PUCHAR q = m2;
    INT charCompareStatus = 0;

    if ( m1 == m2 ) {
        return charCompareStatus;
    }

    while ( len > 0 ) 
    {
        if ( *p != *q )
        {
            charCompareStatus = ( *p > *q ) ? 1 : -1;
            break;
        }
        len--;
        p++;
        q++;
    }

    return charCompareStatus;
}

PVOID
CopyMemoryEx( 
    _Inout_    PVOID  Destination, 
    _In_ CONST PVOID  Source, 
    _In_       SIZE_T Length
) {
    PBYTE Dst = ( PBYTE )Destination;
    PBYTE Src = ( PBYTE )Source;

    while ( Length-- ) 
    {
        *Dst++ = *Src++;
    }

    return Destination;
}

VOID
ZeroMemoryEx( 
    _Inout_ PVOID  Destination, 
    _In_    SIZE_T Size
) {
    PULONG Dst = ( PULONG )Destination;
    SIZE_T Cnt = Size / sizeof( ULONG );

    while ( Cnt > 0 ) 
    {
        *Dst = 0;
        Dst++;
        Cnt--;
    }

    return;
}

// https://github.com/vxunderground/VX-API/blob/main/VX-API/StringLength.cpp
// does not count termianting null byte
SIZE_T
StrLenA( 
    _In_ LPCSTR String
) {
    LPCSTR String2 = NULL;
    
    for ( String2 = String; *String2; ++String2 );
    
    return ( String2 - String );
}

// does not count termianting null wchar
SIZE_T
StrLenW( 
    _In_ LPCWSTR String
) {
    LPCWSTR String2 = NULL;

    for ( String2 = String; *String2; ++String2 );
    
    return ( String2 - String );
}

// https://github.com/vxunderground/VX-API/blob/main/VX-API/StringCompare.cpp
INT
StringCompareA( _In_ LPCSTR String1, _In_ LPCSTR String2 )
{
    for ( ; *String1 == *String2; String1++, String2++ )
    {
        if ( *String1 == '\0' )
            return 0;
    }
    return ( ( *( LPCSTR )String1 < *( LPCSTR )String2 ) ? -1 : +1 );
}

INT
StringCompareW( _In_ LPCWSTR String1, _In_ LPCWSTR String2 )
{
    for ( ; *String1 == *String2; String1++, String2++ )
    {
        if ( *String1 == '\0' )
            return 0;
    }
    return ( ( *( LPCWSTR )String1 < *( LPCWSTR )String2 ) ? -1 : +1 );
}

BOOL
IsStringEqualA( _In_ LPCSTR Str1, _In_ LPCSTR Str2 )
{
    CHAR uStr1[ MAX_PATH ] = { 0 };
    CHAR uStr2[ MAX_PATH ] = { 0 };

    int len1 = StrLenA( Str1 );
    int len2 = StrLenA( Str2 );

    int i = 0;
    int j = 0;

    if ( len1 >= MAX_PATH || len2 >= MAX_PATH ) {
        return FALSE;
    }

    // converting Str1 to uppercase (uStr1)
    for ( i = 0; i < len1; i++ ) {
        uStr1[ i ] = ( Str1[ i ] >= 'a' && Str1[ i ] <= 'z' ) ? ( Str1[ i ] - 0x20 ) : ( Str1[ i ] ); 
    }
    uStr1[ i++ ] = L'\0'; // null termination

    // converting Str2 to uppercase (uStr2)
    for ( j = 0; j < len2; j++ ) {
        uStr2[ j ] = ( Str2[ j ] >= 'a' && Str2[ j ] <= 'z' ) ? ( Str2[ j ] - 0x20 ) : ( Str2[ j ] ); 
    }
    uStr2[ j++ ] = L'\0'; // null termination

    // comparing lowercase strings
    if ( StringCompareA( uStr1, uStr2 ) == 0 ) {
        return TRUE;
    }
    return FALSE;
}

BOOL
IsStringEqualW( _In_ LPCWSTR Str1, _In_ LPCWSTR Str2 )
{
    WCHAR uStr1[ MAX_PATH ] = { 0 };
    WCHAR uStr2[ MAX_PATH ] = { 0 };

    int len1 = StrLenW( Str1 );
    int len2 = StrLenW( Str2 );

    int i = 0;
    int j = 0;

    if ( len1 >= MAX_PATH || len2 >= MAX_PATH ) {
        return FALSE;
    }

    // converting Str1 to uppercase (uStr1)
    for ( i = 0; i < len1; i++ ) {
        uStr1[ i ] = ( Str1[ i ] >= L'a' && Str1[ i ] <= L'z' ) ? ( Str1[ i ] - 0x20 ) : ( Str1[ i ] ); 
    }
    uStr1[ i++ ] = L'\0'; // null termination

    // converting Str2 to uppercase (uStr2)
    for ( j = 0; j < len2; j++ ) {
        uStr2[ j ] = ( Str2[ j ] >= L'a' && Str2[ j ] <= L'z' ) ? ( Str2[ j ] - 0x20 ) : ( Str2[ j ] ); 
    }
    uStr2[ j++ ] = L'\0'; // null termination

    // comparing uppercase strings
    if ( StringCompareW( uStr1, uStr2 ) == 0 ) {
        return TRUE;
    }
    return FALSE;
}

SIZE_T
CharStringToWCharString( 
    _Inout_ PWCHAR Destination, 
    _In_    PCHAR  Source, 
    _In_    SIZE_T MaximumAllowed 
) {
    INT Length = MaximumAllowed;

    while ( --Length >= 0 ) 
    {
        if ( ! ( *Destination++ = *Source++ ) )
        {
            return MaximumAllowed - Length - 1;
        }
    }

    return MaximumAllowed - Length;
}

SIZE_T
WCharStringToCharString( 
    _Inout_ PCHAR  Destination, 
    _In_    PWCHAR Source, 
    _In_    SIZE_T MaximumAllowed
) {
    INT Length = MaximumAllowed;

    while ( --Length >= 0 ) 
    {
        if ( ! ( *Destination++ = *Source++ ) ) 
        {
            return MaximumAllowed - Length - 1;
        }
    }

    return MaximumAllowed - Length;
}

/* This is a simple trick to hide strings from memory :^) */
BYTE __declspec(noinline)
HideChar( BYTE C )
{
    return C;
}