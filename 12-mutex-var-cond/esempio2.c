/**
 * ESEMPIO: NO race condiction
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int myglobal;

// Inizializzazione statica
pthread_mutex_t mymutex = PTHREAD_MUTEX_INITIALIZER;

void* thread_function( void* );
int main( void ) {
    
    pthread_t mythread;
    int i;
    
    if ( pthread_create( &mythread, NULL, thread_function, NULL ) ) {
        printf( "creazione del thread fallita." );
        exit( EXIT_FAILURE );
    }
    
    /**
     * In questo caso il thread principale andrà ad eseguire
     * in mutua esclusione le sue operazioni quindi si risolvono
     * i problemi di race condiction che c'erano
     * 
     * ATTENZIONE: qui si introduce però un'ulteriore osservazione
     * essendo che, i mutex, implementano la loro sincronizzazione
     * controllando l'acceso dei thread ai dati usando il polling
     * essi consumano più CPU del necessario
    */
    for ( i=0; i<20; i++ ) {
        pthread_mutex_lock( &mymutex );
        myglobal = myglobal + 1;
        pthread_mutex_unlock( &mymutex );
        printf( "o" );
        fflush( stdout );
        sleep(1);
    }
    
    if( pthread_join( mythread, NULL ) ) {
        printf( "errore nel join con il thread.\n" );
        exit( EXIT_FAILURE );
    }
    
    printf( "\nmyglobal è uguale a %d\n", myglobal );
    exit( EXIT_SUCCESS );
}

void* thread_function( void* arg ) {

    int i,j;

    /**
     * In questo caso il/i thread andrà/andranno ad eseguire
     * in mutua esclusione le operazioni quindi si risolvono
     * i problemi di race condiction che c'erano
     * 
     * ATTENZIONE: qui si introduce però un'ulteriore osservazione
     * essendo che, i mutex, implementano la loro sincronizzazione
     * controllando l'acceso dei thread ai dati usando il polling
     * essi consumano più CPU del necessario
    */
    for ( i=0; i<20; i++ ) {
        pthread_mutex_lock( &mymutex );
        j = myglobal;
        j = j + 1;
        printf( "." );
        fflush( stdout );
        sleep(1);
        myglobal = j;
        pthread_mutex_unlock( &mymutex );
    }
    return NULL;
}