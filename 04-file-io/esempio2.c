/**
 * ESEMPIO: My Cat
*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../apue/apue.h"

#define BUFFS 4096

int main( void ) {
    int n;
    char buf[ BUFFS ];
    while ( ( n = read( STDIN_FILENO, buf, BUFFS ) ) > 0 )
        if ( write( STDOUT_FILENO, buf, n ) != n )
            printf( "write error" );
        if ( n < 0 )
            printf( "read error" );
    exit(0);
}