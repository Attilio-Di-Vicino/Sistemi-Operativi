/**
 * ESEMPIO:
 * 
 * Produttori - consumatori con semafori basati su memoria
*/

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/stat.h>

#define NBUFF 10
#define MAXNTHREADS 100
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

// sola lettura per prod. e cons.
int nitems, nproducers;

// dati condivisi da prod. e cons.
struct {
    int buff[ NBUFF ];
    int nput;
    int nputval;
    sem_t mutex, nempty, nstored;
} shared;

void* produce( void* );
void* consume( void* );
int main( int argc, char** argv ) {
    
    int i, count[ MAXNTHREADS ];
    pthread_t tid_produce[ MAXNTHREADS ], tid_consume;

    if ( argc != 3 ) {
        printf( "usage: %s <#items> <#producers>", argv[0] );
        exit( EXIT_FAILURE );
    }

    nproducers = MIN( atoi( argv[2] ), MAXNTHREADS );

    nitems = atoi( argv[1] );

    // Creazione semaforo binario
    // La flag 0, quindi il secondo parametro
    // indica che il semaforo è condivisibile solo con un processo
    sem_init( &shared.mutex, 0, 1 );
    
    // Creazione semaforo contatore inizializzato a NBUFF
    sem_init( &shared.nempty, 0, NBUFF );
    
    // Creazione semaforo contatore inizializzato a 0
    sem_init( &shared.nstored, 0, 0 );
    
    // crea tutti i produttori
    for ( i = 0; i < nproducers; i++ ) {
        count[i] = 0;
        pthread_create( &tid_produce[i], NULL, produce, &count[i] );
    }

    // Crea un thread consumatore
    pthread_create( &tid_consume, NULL, consume, NULL );

    printf( "\n" );
    // Aspetta tutti i produttori ed il consumatore
    for ( i = 0; i < nproducers; i++ ) {
        pthread_join( tid_produce[i], NULL );
        printf( "\ncount[ %d ]: %d", i, count[i] );
    }
    pthread_join( tid_consume, NULL );

    // Rimuovi i semafori
    sem_destroy( &shared.mutex );
    sem_destroy( &shared.nempty );
    sem_destroy( &shared.nstored );

    printf( "\n" );
    exit( EXIT_SUCCESS );
}

void* produce( void* arg ) {
    
    int i;
    
    while ( 1 ) {

        // Decremento il semaforo contatore
        sem_wait( &shared.nempty );

        // attende almeno un posto vuoto
        sem_wait( &shared.mutex );
        // Sezione critica
        if ( shared.nput >= nitems ) {

            sem_post( &shared.nempty );
            sem_post( &shared.mutex );
            return NULL; // tutto prodotto
        }

        shared.buff[ shared.nput % NBUFF ] = shared.nputval;
        shared.nput++;
        shared.nputval++;
        printf( "\nProdotto -> [ %d ]: %d", i % NBUFF, i );
        sem_post( &shared.mutex );
        
        sem_post( &shared.nstored );
        // un altro elemento memorizzato
        *( ( int* ) arg ) += 1;
    }
}

void* consume( void* arg ) {
    
    int i;
    
    for ( i = 0; i < nitems; i++ ) {

        sem_wait( &shared.nstored );

        // Attende almeno un elemento memorizzato
        sem_wait( &shared.mutex );
        // Sezione critica
        if ( shared.buff[ i % NBUFF ] != i )
            printf( "error:buff[ %d ]: %d\n", i, shared.buff[ i % NBUFF ] );
        else 
            printf( "\nConsumo -> [ %d ]: %d", i % NBUFF, shared.buff[ i % NBUFF ] );
        sem_post( &shared.mutex );

        sem_post( &shared.nempty );
        // un altro posto libero
    }
    return NULL;
}