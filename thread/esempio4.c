/**
 * ESEMPIO: pthread_once()
 * 
 * Assicura che una routine di inizializzazione sarà eseguita solo una volta senza
 * curarsi di quanti thread nel processo la invocano
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// Come pirma cosa è necessario inizializzare la struttura
pthread_once_t init = PTHREAD_ONCE_INIT;

void init_funz() {

    // { varie inizializzazioni }

    printf( "Inizializzazione effettuata\n" );
}

void* funzione( void* arg ) {

    // in questo caso abbiamo due thread
    // il primo che verrà eseguito eseguire la funzione init_func
    // l'altro thread che arriverà dopo non eseguirà la funzione init_func
    pthread_once( &init, init_funz );

    // { varie istruzioni }

    printf( "Funzione\n" );
}

int main() {

    pthread_t t1, t2;
    int r1, r2;

    r1 = pthread_create( &t1, NULL, funzione, NULL );
    r2 = pthread_create( &t2, NULL, funzione, NULL );

    if ( pthread_join( t1, NULL ) != 0 ) {
        perror( "Errore nella join del thread 1" );
        exit( EXIT_FAILURE );
    }

    if ( pthread_join( t2, NULL ) != 0 ) {
        perror( "Errore nella join del thread 2" );
        exit( EXIT_FAILURE );
    }

    exit( EXIT_SUCCESS );
}