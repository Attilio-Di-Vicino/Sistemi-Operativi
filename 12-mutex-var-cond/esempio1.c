/**
 * ESEMPIO: Race condiction
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// variabile globale int
int myglobal;

void* thread_function( void* );
int main( void ) {

    pthread_t mythread;
    int i;
    
    // Creazione dei thread
    if( pthread_create( &mythread, NULL, thread_function, NULL ) ) {
        printf( "creazione del thread fallita.\n" );
        exit( EXIT_FAILURE );
    }

    /**
     * Ciclo iterativo eseguito dal thread principale
     * quello che ci si aspetta è che esso incrementi
     * il valore di myglobal di 1 * 20
     * 
     * Questo accade, ma essendo una variabile soggetta a race condiction
     * e non usando nessuna tecnica di mutua esclusione il risultato 
     * non risulta essere quello desiderato
    */
    for ( i = 0; i < 20; i++ ) {

        myglobal = myglobal + 1;
        printf( "o" );
        fflush( stdout );
        sleep(1);
    }

    // Attendo che i thread finiscano
    if ( pthread_join( mythread, NULL ) ) {
        printf( "errore nel join dei thread." );
        exit(2);
    }

    printf( "\nmyglobal e’ uguale a %d\n", myglobal );
    exit( EXIT_SUCCESS );
}
    
void* thread_function( void* arg ) {

    int i, j;

    /**
     * Ciclo iterativo eseguito dai possibili thread creati ( in questo caso uno )
     * quello che ci si aspetta è che esso incrementi
     * il valore di myglobal di 1 * 20
     * 
     * Questo accade, ma essendo una variabile soggetta a race condiction
     * e non usando nessuna tecnica di mutua esclusione il risultato 
     * non risulta essere quello desiderato
    */
    for ( i = 0; i < 20; i++ ) {
        
        j = myglobal;
        j = j + 1;
        
        printf( "." );
        fflush( stdout );
        sleep(1);
        
        myglobal = j;
    }
    return NULL;
}