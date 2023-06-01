/**
 * ESEMPIO: Produttore-Consumatore 3
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAXNITEMS 1000000
#define MAXNTHREADS 100

// globali condivise dai thread
int nitems; // sola lettura per prod. e cons.
int buff[ MAXNITEMS ];

struct {
    pthread_mutex_t mutex;
    int nput;
    // indice successivo in cui memorizzare
    int nval; // valore successivo da memorizzare
} put = { PTHREAD_MUTEX_INITIALIZER };

struct {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int nready; // numero a disposizione del cons.
} nready = { PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER};
/* fine globali */

void* produce( void* );
void* consume( void* );
int minimum( int, int );

int main( int argc, char** argv ) {
    int i, nthreads, count[ MAXNTHREADS ];
    pthread_t tid_produce[ MAXNTHREADS ], tid_consume;
    
    if ( argc != 3 ) {
        printf( "usage: prodcons3 <#items> <#threads>" );
        exit( EXIT_FAILURE );
    }
    
    nitems = minimum( atoi( argv[1] ), MAXNITEMS );
    nthreads = minimum( atoi( argv[2] ), MAXNTHREADS );

    // crea tutti i produttori ed un consumatore
    for ( i = 0; i < nthreads; i++ ) {
        count[i] = 0;
        pthread_create( &tid_produce[i], NULL, produce, &count[i] );
    }

    pthread_create( &tid_consume, NULL, consume, NULL );
    
    // aspetta tutti i produttori ed il consumatore
    for ( i = 0; i < nthreads; i++ ) {
        pthread_join( tid_produce[i], NULL );
        printf( "count[%d] = %d\n", i, count[i] );
    }

    pthread_join( tid_consume, NULL );
    exit( EXIT_SUCCESS );
}

void* produce( void* arg ) {

    while(1) {

        pthread_mutex_lock( &put.mutex );
        if ( put.nput >= nitems ) {
            pthread_mutex_unlock( &put.mutex );
            return NULL ; // array pieno
        }

        buff[ put.nput ] = put.nval;
        put.nput++;
        put.nval++;

        pthread_mutex_unlock( &put.mutex );
        
        pthread_mutex_lock( &nready.mutex );
        nready.nready++;
        
        if ( nready.nready > 0 )
        pthread_cond_signal( &nready.cond );
        pthread_mutex_unlock( &nready.mutex );

        *( (int *) arg ) += 1;
    }
}

void* consume( void* arg ) {
    
    int i;
    for ( i = 0; i < nitems; i++ ) {

        pthread_mutex_lock( &nready.mutex );
        
        while ( nready.nready == 0 )
            pthread_cond_wait( &nready.cond, &nready.mutex );
        
        nready.nready--;
        pthread_mutex_unlock(&nready.mutex);
        
        if ( buff[i] != i )
            printf( "buff[%d] = %d\n", i, buff[i] );
    }
    return NULL;
}

int minimum( int a, int b ) {
    return (a > b) ? a: b;
}