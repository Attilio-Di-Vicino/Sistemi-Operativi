/**
 * ESEMPIO:
 * 
 * Produttori - consumatori con semafori con nome
*/

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/stat.h>

#define NBUFF 10
#define SEM_MUTEX "/mutex"
#define SEM_NEMPTY "/nempty"
#define SEM_NSTORED "/nstored"
#define FILE_MODE S_IRUSR | S_IWUSR

// sola lettura per prod. e cons.
int nitems;

// dati condivisi da prod. e cons.
struct {
    int buff[ NBUFF ];
    sem_t *mutex, *nempty, *nstored;
} shared;

void* produce( void* );
void* consume( void* );
int main( int argc, char** argv ) {
    
    pthread_t tid_produce, tid_consume;

    if ( argc != 2 ) {
        printf( "usage: %s <#items>", argv[0] );
        exit( EXIT_FAILURE );
    }

    nitems = atoi( argv[1] );

    // Creazione semaforo binario
    shared.mutex = sem_open( SEM_MUTEX, O_CREAT | O_EXCL, FILE_MODE, 1 );
    
    // Creazione semaforo contatore inizializzato a NBUFF
    shared.nempty = sem_open( SEM_NEMPTY, O_CREAT | O_EXCL, FILE_MODE, NBUFF );
    
    // Creazione semaforo contatore inizializzato a 0
    shared.nstored = sem_open( SEM_NSTORED, O_CREAT | O_EXCL, FILE_MODE, 0 );
    
    // Crea un thread produttore ed un thread consumatore
    pthread_create( &tid_produce, NULL, produce, NULL );
    pthread_create( &tid_consume, NULL, consume, NULL );

    // attende i due thread
    pthread_join( tid_produce, NULL );
    pthread_join( tid_consume, NULL );

    // Chiudere i semafori
    sem_close( SEM_MUTEX );
    sem_close( SEM_NEMPTY );
    sem_close( SEM_NSTORED );

    // rimuove i semafori
    sem_unlink( SEM_MUTEX );
    sem_unlink( SEM_NEMPTY );
    sem_unlink( SEM_NSTORED );

    printf( "\n" );
    exit( EXIT_SUCCESS );
}

void* produce( void* arg ) {
    
    int i;
    
    for ( i = 0; i < nitems; i++ ) {

        // Decremento il semaforo contatore
        sem_wait( shared.nempty );

        // attende almeno un posto vuoto
        sem_wait( shared.mutex );
        // Sezione critica
        // memorizza i nel buffer circolare
        shared.buff[ i % NBUFF ] = i;
        printf( "\nProdotto -> [ %d ]: %d", i % NBUFF, i );
        sem_post( shared.mutex );

        // Incremento il semaforo contatore
        sem_post( shared.nstored );
        // un altro elemento è disponibile
    }
    return NULL;
}

void* consume( void* arg ) {
    
    int i;
    
    for ( i = 0; i < nitems; i++ ) {

        // Decremento il semaforo contatore
        sem_wait( shared.nstored );
        // attende almeno un elemento
        sem_wait( shared.mutex );
        // Sezione critica
        if ( shared.buff[ i % NBUFF ] != i )
            printf( "buff[ %d ]: %d\n", i, shared.buff[ i % NBUFF ] );
        else 
            printf( "\nConsumo -> [ %d ]: %d", i % NBUFF, shared.buff[ i % NBUFF ] );
        sem_post( shared.mutex );

        // Incremento il semaforo contatore
        sem_post( shared.nempty );
        //un altro posto libero
    }
    return NULL;
}