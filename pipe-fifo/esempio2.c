#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

// direttiva al processore
// costante utilizzata per il nome della fifo
#define FIFO_NAME "/tmp/my_fifo"

int main( int argc, char *argv[] ) {
    
    int res;
    int open_mode = 0;
    int i;

    if ( argc < 2 ) {
        printf( "Uso: %s <combinazioni di O_RDONLY O_WRONLY O_NONBLOCK>\n", argv[0] );
        exit( EXIT_FAILURE );
    }

    // Impostiamo il valore di open_mode dagli argomenti.
    for( i = 1; i < argc; i++ ) {

        if ( strncmp( *++argv, "O_RDONLY", 8 ) == 0 ) 
            open_mode |= O_RDONLY;

        if ( strncmp( *argv, "O_WRONLY", 8 ) == 0 )
            open_mode |= O_WRONLY;

        if ( strncmp( *argv, "O_NONBLOCK", 10 ) == 0 )
            open_mode |= O_NONBLOCK;
    }
    
    // Se la FIFO non esiste la creiamo. Poi viene aperta
    // questa condizione controlla se il file FIFO non esiste o non può essere letto
    // in tal caso la crea
    if ( access( FIFO_NAME, F_OK ) == -1 ) {

        res = mkfifo( FIFO_NAME, 0777 );

        if ( res != 0 ) {
            printf( "Non posso creare la FIFO %s\n", FIFO_NAME );
            exit( EXIT_FAILURE );
        }
    }

    printf( "Processo %d apre la FIFO\n", getpid() );
    res = open( FIFO_NAME, open_mode );

    printf( "Risultato processo %d: %d\n", getpid(), res );
    sleep(5);

    if ( res != -1 ) 
        close( res );
    
    printf( "Processo %d terminato\n", getpid() );
    exit( EXIT_SUCCESS );
}

/**
 * Compilare il file
 * ed eseguire in background
 * ./a.out O_RDONLY &
 * ./a.out O_WRONLY
 * 
 * OUTPUT
 * 
 * [2] 13788
 * Processo 13788 apre la FIFO
 * Processo 13840 apre la FIFO
 * Risultato processo 13840: 3
 * Risultato processo 13620: 3
 * Risultato processo 13788: 3
 * Processo 13788 terminato
 * Processo 13620 terminato
 * Processo 13840 terminato
 * [1]-  Completato              ./a.out O_RDONLY
 * [2]+  Completato              ./a.out O_RDONLY
*/