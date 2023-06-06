/**
 * ESEMPIO:
 * 
 * Semaforo POSIX con nome
*/

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>

int main( int argc, char** argv ) {

    sem_t* sem;
    int val;

    if (argc != 2  ) {
        fprintf( stderr, "usage: %s <name>\n", argv[0] );
        exit( EXIT_FAILURE );
    }

    // Aprertura semaforo
    sem = sem_open( argv[1], O_CREAT, 0644, 1 );
    if ( sem == SEM_FAILED ) {
        perror( "Errore nell'apertura del semaforo" );
        exit( EXIT_FAILURE );
    }
    printf( "\nSemaforo creato e aperto con successo." );

    // Wait semaforo
    sem_wait( sem );

    // GetValue semaforo
    sem_getvalue( sem, &val );
    printf( "\npid %ld has semaphore, value = %d\n", ( long ) getpid(), val );

    // Post semaforo
    sem_post( sem );

    // Chiudi e rimuovi il semaforo
    if ( sem_close( sem ) == -1 ) {
        perror( "Errore nella chiusura del semaforo" );
        exit( EXIT_FAILURE );
    }

    if ( sem_unlink( argv[1] ) == -1 ) {
        perror( "Errore nella rimozione del semaforo" );
        exit( EXIT_FAILURE );
    }

    printf( "\nSemaforo chiuso e rimosso con successo." );

    printf( "\n" );
    exit( EXIT_SUCCESS );
}