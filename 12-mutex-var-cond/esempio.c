#include <pthread.h>
#include <stdio.h>

// Inizializzazione Statica
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* threadFunction( void* arg ) {

    /**
     * Questa funzione tenta di acquisire il mutex specificato tramite il puntatore mutex,
     * ma se il mutex è già acquisito da un altro thread, la funzione restituisce
     * immediatamente con un valore diverso da zero, indicando che il tentativo di acquisizione non è riuscito.
     * Altrimenti, se il mutex è disponibile e viene acquisito con successo, la funzione restituisce zero.
    */
    int result = pthread_mutex_trylock( &mutex );
    if ( result == 0 ) {

        // Il mutex è stato acquisito con successo
        printf( "Mutex acquisito dal thread\n" );
        
        // Esegui le operazioni desiderate
        
        pthread_mutex_unlock( &mutex );
        printf( "Mutex rilasciato dal thread\n" );
    } else {

        // Il mutex è già acquisito da un altro thread
        printf( "Mutex già acquisito da un altro thread\n" );
    }

    return NULL;
}

int main() {

    pthread_t thread;
    pthread_create( &thread, NULL, threadFunction, NULL );
    
    // { Esegui altre operazioni }
    
    pthread_join( thread, NULL );

    return 0;
}
