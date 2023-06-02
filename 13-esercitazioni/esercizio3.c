/**
 * ESERCIZIO: 3
 * 
 * Scrivere un programma C e Posix sotto Linux che,
 * utilizzando la libreria Pthread crei due Thread "somma1" e "somma2",
 * entrambi accedono alle variabili test.a e test.b di una struttura dati test condivisa
 * incrementandole di 1 per 10 volte.
 * 
 * Si utilizzi come meccanismo di gestione della concorrenza
 * quello della "mutua esclusione per struttura",
 * dove la struttura mutex va allocata dinamicamente
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define ITER 10

// Struttura test
typedef struct {
    int a;
    int b;
} test;

// Mutex
pthread_mutex_t mutex;

void* routine( void* argv ) {

    // Cast della struttura
    test* parameters = ( test* ) argv;

    // Eseguo il ciclo per l'incremento
    for ( int i = 0; i < ITER; i++ ) {
        // incremento in mutua esclusione
        pthread_mutex_lock( &mutex );
        parameters->a++;
        parameters->b++;
        pthread_mutex_unlock( &mutex );
    }
    pthread_exit( EXIT_SUCCESS );
}

int main() {

    // Inizializzazione struttura condivisa
    test* parameters = ( test* ) malloc( sizeof( test ) );
    parameters->a = 0;
    parameters->b = 0;

    // Allocazione mutex
    pthread_mutex_init( &mutex, NULL );

    // Thread
    pthread_t somma1, somma2;

    // Creazione Thread somma1
    if ( pthread_create( &somma1, NULL, routine, ( void* ) parameters ) != 0 ) {
        perror( "Errore nella creazione del Thread somma1" );
        exit( EXIT_FAILURE );
    }

    // Creazione Thread somma2
    if ( pthread_create( &somma2, NULL, routine, ( void* ) parameters ) != 0 ) {
        perror( "Errore nella creazione del Thread somma2" );
        exit( EXIT_FAILURE );
    }

    // Attesa Thread somma1
    if ( pthread_join( somma1, NULL ) != 0 ) {
        perror( "Errore nella join del Thread somma1" );
        exit( EXIT_FAILURE );
    }

    // Attesa Thread somma2
    if ( pthread_join( somma2, NULL ) != 0 ) {
        perror( "Errore nella join del Thread somma2" );
        exit( EXIT_FAILURE );
    }

    // Deallocazione mutex
    pthread_mutex_destroy( &mutex );
    
    // Verifico l'output
    printf( "\nIl valore di test dopo l'esecuzione" );
    printf( "\ndi somma1 e somma2 è:" );
    printf( "\n1- test.a: %d\n2- test.b: %d\n", parameters->a, parameters->b );
    exit( EXIT_SUCCESS );
}
