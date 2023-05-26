#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * ATTENZIONE: in questo semplice esempio
 * non viene sincronizzata la stampa a video dei due thread
 * esso è una risorsa condivisa di conseguenza c'è bisogno di una sincronizzazione
*/
void* routine( void* args ) {

    // restituisce l'ID del thread invocante
    printf( "\nHello i'm thread %ld", pthread_self() );
}

int main() {

    // identificativo thread 
    // NB: in alcune implementazioni pthread_t è un puntatore ad una struttura
    pthread_t th[2];

    // Creazione del primo thread
    // restuisce 0 se ok, numero di errore se fallisce
    if ( pthread_create( &th[0], NULL, routine, NULL ) != 0 ) {
        perror( "Errore nella creazione thread 1" );
        exit( EXIT_FAILURE );
    }

    // Creazione del primo thread
    // restuisce 0 se ok, numero di errore se fallisce
    if ( pthread_create( &th[1], NULL, routine, NULL ) != 0 ) {
        perror( "Errore nella creazione thread 2" );
        exit( EXIT_FAILURE );
    }

    // Effettuiamo la join sul thread appena creato 
    // per assicurarci ch esso venga eseguito
    // restituisce 0 se è ok, numero di errore se fallisce
    if ( pthread_join( th[0], NULL ) != 0 ) {
        perror( "Errore nella join del thread 1" );
        exit( EXIT_FAILURE );
    }

    // Effettuiamo la join sul thread appena creato 
    // per assicurarci ch esso venga eseguito
    // restituisce 0 se è ok, numero di errore se fallisce
    if ( pthread_join( th[1], NULL ) != 0 ) {
        perror( "Errore nella join del thread 2" );
        exit( EXIT_FAILURE );
    }

    // per confrontare gli id dei thread è necessaria una funzione
    // restituisce un valore non nullo se uguali, 0 altrimenti
    int rs = pthread_equal( th[0], th[1] );

    printf( "\nRisultato pthread_equal: %d\n", rs );
    exit( EXIT_SUCCESS );
}