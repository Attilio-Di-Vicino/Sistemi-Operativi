/**
 * ESEMPIO: Produttore-Consumatore 2
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAXNITEMS 1000000
#define MAXNTHREADS 100

int nitems; //sola lettura per prod. e cons.

struct {
    pthread_mutex_t mutex;
    int buff[ MAXNITEMS ];
    int nput;
    int nval;
} shared = { PTHREAD_MUTEX_INITIALIZER };

void* produce( void* );
void* consume( void* );
int minimum( int, int );

int main( int argc, char** argv ) {

    int i, nthreads, count[ MAXNTHREADS ];
    pthread_t tid_produce[ MAXNTHREADS ], tid_consume;

    if ( argc != 3 ) {
        printf( "usage: prodcons2 <#items> <#threads>" );
        exit( EXIT_FAILURE );
    }
    
    nitems = minimum( atoi( argv[1] ), MAXNITEMS );
    nthreads = minimum( atoi( argv[2] ), MAXNTHREADS );
    
    for ( i = 0; i < nthreads; i++ ) {
        count[i] = 0;
        pthread_create( &tid_produce[i], NULL, produce, &count[i] );
    }

    pthread_create( &tid_consume, NULL, consume, NULL );
    
    // aspetta tutti i produttori e il consumatore
    for ( i = 0; i < nthreads; i++ ) {
        pthread_join( tid_produce[i], NULL );
        printf( "count[%d] = %d\n", i, count[i] );
    }

    pthread_join( tid_consume, NULL );
    exit( EXIT_SUCCESS );
}

void* produce( void* arg ) {
    
    while (1) {

        pthread_mutex_lock( &shared.mutex );
        if ( shared.nput >= nitems ) {
        pthread_mutex_unlock( &shared.mutex );
        return NULL; // array pieno
    }

    shared.buff[ shared.nput ] = shared.nval;
    shared.nput++;
    shared.nval++;
    pthread_mutex_unlock( &shared.mutex );
    *( ( int* ) arg ) += 1;
    }
}

void consume_wait( int i ) {

    while (1) {

        pthread_mutex_lock( &shared.mutex );
        
        if ( i < shared.nput) {
            pthread_mutex_unlock( &shared.mutex );
            return; // un elemento è pronto
        }
        pthread_mutex_unlock( &shared.mutex );
    }
}

void* consume( void* arg ) {

    int i;
    
    for ( i = 0; i < nitems; i++ ) {
        consume_wait(i);
        if ( shared.buff[i] != i )
            printf( "buff[%d] = %d\n", i, shared.buff[i] );
    }
    
    return NULL;
}

int minimum( int a, int b ) {
    return (a > b) ? a: b;
}