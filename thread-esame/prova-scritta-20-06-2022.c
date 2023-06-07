/**
 * ESAME: Prova Scritta 20/06/2022
*/

/**
 * 9 punti ( obbligatorio )
 * 
 * Scrivere un programma C e Posix che prende da riga di comando N interi
 * compresi tra 5 e 20. Il valore di N è constante ed indicato nel sorgente.
 * 
 * Il programma avvia N thread che hanno il seguente comportamento:
 * 
 *  a) Ogni thread va in sospensione ( invocando la funzione sleep() ) per un numero
 *     di secondi pari al valore del corrispondente parametro e poi stampa il suo indice
 *  
 *  b) Alla fine del periodo di attesa ( sleep ), ogni thread stampa un messaggio: "Thread #: svegliato!".
 *  
 *  c) Tutti i thread si sincronizzano tra di loro ( utilizzando i semafori basati su memoria ).
 * 
 *  d) dopo aver atteso il termine di tutti gli altri thread, ciascun thread scrive sullo schermo 
 *     che ha terminato ( "Thread #: terminato..." ).
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

struct sync {
    sem_t sem_CS;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int n;
    int count;
} shared;

void* routine( void* arg ) {
    pthread_t* myTid = ( pthread_t* ) arg;

    // Attendo tid secondi
    sleep( *myTid );

    // decremento il semaforo binario 
    // implementando una sezione critica
    sem_wait( &shared.sem_CS );
    printf( "\nThread %ld: svegliato!", *myTid );
    shared.count++;
    sem_post( &shared.sem_CS );

    if ( *myTid == shared.n ) {
        // Sono l'utlimo thread
        pthread_cond_broadcast( &shared.cond );
        pthread_mutex_lock( &shared.mutex );
        printf( "\nThread %ld: terminato...", *myTid );
        pthread_mutex_unlock( &shared.mutex );
    } else {
        // Non sono l'utlimo thread
        pthread_mutex_lock( &shared.mutex );
        while ( shared.count != ( shared.n + 1 ) )
            pthread_cond_wait( &shared.cond, &shared.mutex );
        printf( "\nThread %ld: terminato...", *myTid );
        pthread_mutex_unlock( &shared.mutex );
    }
    pthread_exit( NULL );
}

int main( int argc, char* argv[] ) {

    if ( argc != 2 ) {
        printf( "\nusage: %s <#N>", argv[0] );
        exit( EXIT_FAILURE );
    }

    // Prendo N
    const int N = atoi( argv[1] );
    printf( "\nN: %d", N );

    // Inizializzazione semaforo binario ad 1, mutex, var cond, n e count
    sem_init( &shared.sem_CS, 0, 1 );
    pthread_mutex_init( &shared.mutex, NULL );
    pthread_cond_init( &shared.cond, NULL );
    shared.n = N - 1;
    shared.count = 0;

    // Thread
    pthread_t th[N];

    // Creazione thread
    for ( int i = 0; i < N; i++ ) {
        int* tid = malloc( sizeof( int ) );
        *tid = i;
        if ( pthread_create( &th[i], NULL, routine, ( void* ) tid ) != 0 ) {
            perror( "Errore nella creazione dei thread" );
            exit( EXIT_FAILURE );
        }
    }

    // Attesa thread
    for ( int i = 0; i < N; i++ ) {
        if ( pthread_join( th[i], NULL ) != 0 ) {
            perror( "Errore nella join dei thread" );
            exit( EXIT_FAILURE );
        }
    }

    // destroy mutex e var cond
    pthread_mutex_destroy( &shared.mutex );
    pthread_cond_destroy( &shared.cond );

    printf( "\n" );
    exit( EXIT_SUCCESS );
}