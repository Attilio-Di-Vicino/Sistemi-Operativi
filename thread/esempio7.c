#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM_THREADS 8

// Questa routine riceve in ingresso l'id del thread

void* PrintHello( void* threadid ) {
    int* id_ptr, taskid;

    sleep(1);
    
    id_ptr = ( int* ) threadid;
    taskid = *id_ptr;
    
    printf( "Thread %d\n", taskid );
    pthread_exit( NULL );
}

int main( int argc, char* argv[] ) {

    pthread_t threads[ NUM_THREADS ];
    
    int* taskids[ NUM_THREADS ];
    int rc, t;
    
    for( t = 0; t < NUM_THREADS; t++ ) {
        
        // indirizzo di un intero
        taskids[t] = ( int* ) malloc( sizeof( int ) );
        *taskids[t] = t;
        
        // l'intero
        printf( "Creating thread %d\n", t );
        rc = pthread_create( &threads[t], NULL, PrintHello, ( void* ) taskids[t] );
        
        if ( rc ) {
            printf( "ERROR; return code from pthread_create() is %d\n", rc );
            exit( -1 );
        }
    }
    pthread_exit( NULL );
}