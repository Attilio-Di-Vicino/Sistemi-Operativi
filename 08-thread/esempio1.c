#include "../apue/apue.h"
#include <pthread.h>
#include <stdlib.h>

pthread_t ntid;

void printids( const char* s ) {
    
    pid_t pid;
    pthread_t tid;
    
    pid = getpid();
    tid = pthread_self();
    
    printf( "%s pid %lu tid %lu (0x%lx)\n", s, ( pid_t ) pid, 
            ( long unsigned int ) tid, ( long unsigned int ) tid );
}

void* thr_fn( void* arg ) {

    printids( "nuovo thread: " );
    return ( ( void* ) 0 );
}

int main( void ) {

    int err = pthread_create( &ntid, NULL, thr_fn, NULL );
    
    if ( err != 0 ) {
        fprintf( stderr, "non posso creare il thread: %s \n", strerror( err ) );
        exit( EXIT_FAILURE );
    }
    
    printids( "thread principale:" );
    sleep(1);
    exit( EXIT_SUCCESS );
}