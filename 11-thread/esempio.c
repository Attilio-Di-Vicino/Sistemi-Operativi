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

    // Se un qualsiasi thread chiama exit, _Exit o
    // exit, allora l’intero processo termina
    // per la terminazione del singolo thread si utilizza pthread_exit
    pthread_exit( ( void* ) EXIT_SUCCESS );
}

int main() {

    // identificativo thread 
    // NB: in alcune implementazioni pthread_t è un puntatore ad una struttura
    pthread_t th[2];

    // valore di exit del thread
    void* tret[2];

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
    if ( pthread_join( th[0], &tret[0] ) != 0 ) {
        perror( "Errore nella join del thread 1" );
        exit( EXIT_FAILURE );
    }

    // Effettuiamo la join sul thread appena creato 
    // per assicurarci ch esso venga eseguito
    // restituisce 0 se è ok, numero di errore se fallisce
    if ( pthread_join( th[1], &tret[1] ) != 0 ) {
        perror( "Errore nella join del thread 2" );
        exit( EXIT_FAILURE );
    }

    // per confrontare gli id dei thread è necessaria una funzione
    // restituisce un valore non nullo se uguali, 0 altrimenti
    int resultEqual = pthread_equal( th[0], th[1] );

    // Un thread può essere cancellato utilizzando la seguente funzione
    // restituisce 0 se ok, numero di errore se fallisce
    int resultCancel = pthread_cancel( th[0] );

    // Un thread può essere distaccato utilizzando la seguente funzione
    // restituisce 0 se ok, numero di errore se fallisce
    int resultDetach = pthread_detach( th[0] );

    printf( "\n\nRisultato pthread_equal: %d", resultEqual );
    printf( "\nRisultato pthread_cancel: %d", resultCancel );
    printf( "\nRisultato pthread_detach: %d", resultDetach );
    printf( "\nRisultato di uscita del thread 1: %ld", ( long ) tret[0] );
    printf( "\nRisultato di uscita del thread 2: %ld\n", ( long ) tret[1] );

    exit( EXIT_SUCCESS );
}