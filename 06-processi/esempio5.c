
#include "../apue/apue.h"

int main() {

    int pid, status;
    pid = fork();
    if ( pid == 0 ) {
        execl( "/bin/ls", "ls", "-l", ( char * ) 0 );
        printf( "exec fallita!\n" );
        exit(1);
    } else if ( pid > 0 ) {
        pid = wait( &status );
        /* gestione dello stato.. */
        exit(0);
    } else 
        printf( "fork fallita!" );
    exit(2);
}