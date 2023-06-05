#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main( int argc, char* argv[] ) {

    if ( argc != 2 ) {
        printf( "usage: a.out <pathname>" );
        exit( EXIT_FAILURE );
    }

    if ( access( argv[1], R_OK ) < 0 ) 
        printf( "access error for %s", argv[1] );
    else
        printf( "read access OK\n" );
        
    if ( open( argv[1], O_RDONLY ) < 0 )
        printf( "Open error for %s", argv[1] );
    else
        printf( "open for reading OK\n" );

    exit( EXIT_SUCCESS );
}