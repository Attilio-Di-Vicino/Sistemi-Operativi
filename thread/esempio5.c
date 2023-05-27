/**
 * ESEMPIO: pthread_kill()
 * 
 * Essa è la funziona equivalente alla kill eseguita 
 * su di un porcesso
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

int segnale;

/**
 * All'interno del gestore del segnale
 * possiamo mettere tutto il codice che vogliamo
 * ma è consigliato comunque di non farlo
 * e quindi di fare operazioni semplici
*/
static void signal_handler( int signo ) {
    segnale = signo;
}

void* thread_function( void *arg ) {

    int thread_id = *( ( int* ) arg );

    printf( "Thread %d in esecuzione.\n", thread_id );

    // Registriamo il gestore di segnale per il thread corrente
    if ( signal( SIGUSR1, signal_handler ) == SIG_ERR ) {
        printf( "Non sono riuscito a catturare il segnael." );
    }

    // in questo caso è sono un esempio di come iniviare un segnale
    // ad un unico thread
    if ( thread_id == 2 ) {
        pause();
        if ( segnale == SIGUSR1 ) {
            printf( "Ho ricevuto il segnale Thread: %d!\n", thread_id );
        }
    }

    // Inseriamo una breve pausa per simulare l'esecuzione del thread
    sleep( 2 );

    printf( "Thread %d terminato.\n", thread_id );

    return NULL;
}

int main() {

    pthread_t thread1, thread2, thread3, thread4;
    int thread_id1 = 1, thread_id2 = 2, thread_id3 = 3, thread_id4 = 4;

    // Creiamo i thread
    pthread_create( &thread1, NULL, thread_function, ( void* ) &thread_id1 );
    pthread_create( &thread2, NULL, thread_function, ( void* ) &thread_id2 );
    pthread_create( &thread3, NULL, thread_function, ( void* ) &thread_id3 );
    pthread_create( &thread4, NULL, thread_function, ( void* ) &thread_id4 );

    // Inseriamo una breve pausa per garantire l'avvio dei thread
    sleep(1);

    // Inviamo il segnale al thread2 usando pthread_kill
    pthread_kill( thread2, SIGUSR1 );

    // Attendiamo la terminazione dei thread
    pthread_join( thread1, NULL );
    pthread_join( thread2, NULL );
    pthread_join( thread3, NULL );
    pthread_join( thread4, NULL );

    printf( "Tutti i thread terminati.\n" );

    return 0;
}
