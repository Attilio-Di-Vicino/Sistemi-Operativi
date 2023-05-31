/**
 * ESEMPIO: produttore - consumatore 1
*/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

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
    
    if ( argc != 3  ) {
        printf( "usage: prodcons <#items> <#threads>" );
        exit(-1);
    }

    nitems = minimum( atoi( argv[1] ), MAXNITEMS );
    nthreads = minimum( atoi( argv[2] ), MAXNTHREADS );
    
    /* inizia tutti i thread produttore */
    for ( i = 0; i < nthreads; i++ ) {
        count[i] = 0;
        pthread_create( &tid_produce[i], NULL, produce, &count[i] );
    }

    /* aspetta tutti i thread produttore */
    for ( i = 0; i < nthreads; i++ ) {
        pthread_join( tid_produce[i], NULL );
        printf( "count[%d] = %d\n", i, count[i] );
    }

    /* inizia e poi aspetta il thread consumatore */
    pthread_create( &tid_consume, NULL, consume, NULL );
    pthread_join( tid_consume, NULL );
    
    exit( EXIT_SUCCESS );
}

void* produce( void* arg ) {

    while (1) {
        pthread_mutex_lock( &shared.mutex );

        if ( shared.nput >= nitems ) {
            pthread_mutex_unlock( &shared.mutex );
            return NULL ; /* array pieno */
        }

        shared.buff[ shared.nput ] = shared.nval;
        shared.nput++;
        shared.nval++;
        pthread_mutex_unlock( &shared.mutex );
        *( ( int* ) arg) += 1;
    }
}

void* consume( void* arg ) {
    int i;

    for ( i = 0; i < nitems; i++ ) {
        if ( shared.buff[i] != i )
            printf( "buff[%d] = %d\n", i, shared.buff[i] );
    }
    return NULL;
}

int minimum( int a, int b ) {
    return (a > b) ? a: b;
}