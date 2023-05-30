#include <sys/types.h>
#include <sys/stat.h>

int main( int argc, char *argv[] ) {
    
    int i;
    struct stat buf;
    char* ptr;
    
    for ( i = 1; i < argc; i++ ) {

        printf( "%s: ", argv[i] );
        
        if ( lstat( argv[i], &buf ) < 0 ) {
            printf( "lstat error\n" );
            continue;
        }

        if ( S_ISREG( buf.st_mode ) )
            ptr = "regular";
        else if ( S_ISBLK( buf.st_mode ) ) 
            ptr = "block special";
        else 
            ptr = "** unknown mode **";
        printf( "%s\n", ptr );
    }
    exit(0);
}