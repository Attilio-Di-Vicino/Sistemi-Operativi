/* Stampa vari user e group ID per un processo */

#include <stdio.h>
#include <unistd.h>
#include "../apue/apue.h"

int main( void ) {

    printf( "Mio real user ID: %5d\n", ( uid_t ) getuid() );
    printf( "Mio effective user ID:%5d\n", ( uid_t ) geteuid() );
    printf( "Mio real group ID:%5d\n", ( gid_t ) getgid() );
    printf( "Mio effective group ID:%5d\n", ( gid_t ) getegid() );
    return 0;
}