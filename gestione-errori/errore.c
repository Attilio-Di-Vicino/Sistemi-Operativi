#include <errno.h>
#include <stdio.h>
#include <string.h>

/**
 * Esempio di una gestione di errore
 * la funzione strerror( EACCES ) stampa il messagio
 * inerente a quell'errore
*/
int main( int argc, char *argv[] ) {

    fprintf( stderr, "EACCES: %s\n", strerror( EACCES ) );
    errno = ENOENT;
    perror( argv[ 0 ] );
    exit( 0 );
}