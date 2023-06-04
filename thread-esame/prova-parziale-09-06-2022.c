/**
 * PARZIALE: Prova Scritta 14/07/2022
*/

/**
 * 9 punti ( obbligatorio )
 * 
 * Scrivere un programma in C che ( in ambiente Linux e utilizzando la libreria pthread ) crei 2 thread che eseguono
 * la funzione "incrementa" che a sua volta accede alle variabili glob.a e glob.b di una struttura dati condivisa glob e
 * ne incrementi il loro valore di 1 per 100 volte. Al termine, quando i due thread avranno terminato con gli 
 * incrementi, il thread principale stamperà a video i valori delle variabili test.a e test.b. Per la gestione della
 * sincronizzazione si utilizzi i mutex allocati dinamicamente.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUMTHREAD 2
#define NUMITER 100

// Struttra globale
struct globale {
    int a;
    int b;
} glob = { 0, 0 };

// Mutex
pthread_mutex_t mutex;

void printGlob() {
    printf( "\nStruttura Globale:" );
    printf( "\nglob.a: %d", glob.a );
    printf( "\nglob.b: %d", glob.b );
}

/**
 * La funzione incrementa si occupa
 * di incrementare il valore di a e b 
 * contenuti nella struttura glob, la quale è globale
 * essendo globale ed richiamata più volte da più thread contemporaneamente
 * essa è soggetta a race condiction, di conseguenza si utilizza un mutex
 * per l'implementazione della mutua esclusione
*/
void incrementa() {
    // In questo caso la struttura globale
    // è soggetta a race condiction di conseguenza
    // mi avvalgo di un mutex per risolvere il problema
    // Aquisisco il mutex
    pthread_mutex_lock( &mutex );
    // Sezione critica
    // Incremento in mutua esclusione
    glob.a++;
    glob.b++;
    // Rilascio il mutex
    pthread_mutex_unlock( &mutex );
}

/**
 * La routine è una funzione che eseguono i thread
 * essa a sua volta si avvale della funzione incrementa
 * per effettuare le opportune operazione sulla struttra globale glob
*/
void* routine() {
    for ( int i = 0; i < NUMITER; i++ ) {
        incrementa();
    }
}

void main() {

    // Allocazione dinamica del mutex
    pthread_mutex_init( &mutex, NULL );

    // Thread
    pthread_t th[ NUMTHREAD ];

    // Creazione Thread
    for ( int i = 0; i < NUMTHREAD; i++ ) {
        if ( pthread_create( &th[i], NULL, routine, NULL ) != 0 ) {
            perror( "Errore nella creazione dei thread" );
            exit( EXIT_FAILURE );
        }
    }

    // Attesa Thread
    for ( int i = 0; i < NUMTHREAD; i++ ) {
        if ( pthread_join( th[i], NULL ) != 0 ) {
            perror( "Errore nella join dei thread" );
            exit( EXIT_FAILURE );
        }
    }

    // Una volta che i thread hanno concluso il thread principale
    // proveddera alla stampa di glob, anche se glob è una struttura condivisa
    // a questo punto del processo non è soggetta a race condiction
    printGlob();

    // Deallocazione e uscita dal programma
    pthread_mutex_destroy( &mutex );
    printf( "\n" );
    exit( EXIT_SUCCESS );
}