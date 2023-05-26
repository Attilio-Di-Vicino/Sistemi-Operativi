#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void* routine( void* args ) {

    // restituisce l'ID del thread invocante
    printf( "\nHello I'm thread %ld", pthread_self() );
}

int main() {

    // identificativo thread 
    // NB: in alcune implementazioni pthread_t è un puntatore ad una struttura
    pthread_t tid[2];

    // Creazione del primo thread
    // restuisce 0 se ok, numero di errore se fallisce
    if ( pthread_create( &tid[0], NULL, routine, NULL ) != 0 ) {
        perror( "Errore nella creazione thread 1" );
        exit( EXIT_FAILURE );
    }

    // Creazione del primo thread
    // restuisce 0 se ok, numero di errore se fallisce
    if ( pthread_create( &tid[1], NULL, routine, NULL ) != 0 ) {
        perror( "Errore nella creazione thread 2" );
        exit( EXIT_FAILURE );
    }

    // per confrontare gli id dei thread è necessaria una funzione
    // restituisce un valore non nullo se uguali, 0 altrimenti
    int rs = pthread_equal( tid[0], tid[1] );

    printf( "\nRisultato pthread_equal: %d\n", rs );
    exit( EXIT_SUCCESS );
}