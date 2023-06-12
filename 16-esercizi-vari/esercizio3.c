/**
 * ESERCIZIO: 3
 * 
 * Scrivere un programma C in ambiente Linux che facendo uso della libreria Pthread
 * realizzi il seguente comportamento: 
 * 
 *  1. un master thread inizializza una variabile globale a zero,
 *     crea un thread produttore ed un thread consumatore e, 
 *     in un ciclo infinito, stampa il valore della variabile globale.
 *   
 *  2. Il produttore incrementa, ad ogni passo,
 *     la variabile globale di due unità e dorme per un secondo. 
 * 
 *  3. Il consumatore decrementa la variabile globale
 *     di una unità e dorme per un secondo. 
 * 
 *  4. Il processo termina quando la variabile globale è maggiore
 *     di un valore intero passato da riga di comando. 
 * 
 * Utilizzare un semaforo Posix per la sincronizzazione.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define TIMESLEEP 1

// Struttura globale per la sincronizzazione
struct sync {
    sem_t sem_CS; // semaforo binario
    sem_t full; // semaforo contatore
    sem_t empty; // semaforo contatore
} shared;

// Variabili globali
int N;
int glob;

void* produce( void* arg ) {
    while ( 1 ) {

        // Decremento lo spazio vuoto
        sem_wait( &shared.empty );
        // Entro in sezione critica
        sem_wait( &shared.sem_CS );
        // Controllo prima glob
        if ( glob > N ) {
            sem_post( &shared.sem_CS );
            sem_post( &shared.empty );
            pthread_exit( NULL );
        }
        glob += 2;
        printf( "\nProdotto 2 elementi -> glob: %d", glob );
        // Esco dalla sezione critica
        sem_post( &shared.sem_CS );
        sleep( TIMESLEEP );
        // Incremento lo spazio pieno
        sem_post( &shared.full );
    }
}

void* consume( void* arg ) {
    while ( 1 ) {

        // Decremento lo spazio pieno
        sem_wait( &shared.full );
        // Entro in sezione critica
        sem_wait( &shared.sem_CS );
        // Controllo prima glob
        if ( glob > N ) {
            sem_post( &shared.sem_CS );
            sem_post( &shared.full );
            pthread_exit( NULL );
        }
        printf( "\nConsumato 1 elemento -> glob: %d", glob-- );
        // Esco dalla sezione critica
        sem_post( &shared.sem_CS );
        sleep( TIMESLEEP );
        // Incremento lo spazio pieno
        sem_post( &shared.empty );
    }
}

int main( int argc, char* argv[] ) {

    // Verifica parametri in input da riga di comando
    if ( argc != 2 ) {
        printf( "\nusage: %s <#N>", argv[0] );
        exit( EXIT_FAILURE );
    }

    // Inizializzazione variabili globali
    N = atoi( argv[1] );
    glob = 0;

    printf( "\nN: %d, glob: %d", N, glob );

    // Inizializzazione semafori
    sem_init( &shared.sem_CS, 0, 1 ); // binario
    sem_init( &shared.full, 0, 0 ); // contatore
    sem_init( &shared.empty, 0, N ); // contatore

    pthread_t producer, consumer;

    if ( pthread_create( &producer, NULL, produce, NULL ) != 0 ) {
        perror( "Errore nella creazione del produttore" );
        exit( EXIT_FAILURE );
    }
    if ( pthread_create( &consumer, NULL, consume, NULL ) != 0 ) {
        perror( "Errore nella creazione del consumatore" );
        exit( EXIT_FAILURE );
    }
    if ( pthread_join( producer, NULL ) != 0 ) {
        perror( "Errore nella join del produttore" );
        exit( EXIT_FAILURE );
    }
    if ( pthread_join( consumer, NULL ) != 0 ) {
        perror( "Errore nella join del consumatore" );
        exit( EXIT_FAILURE );
    }

    // Deallocazione semafori
    sem_destroy( &shared.sem_CS ); // binario
    sem_destroy( &shared.full ); // contatore
    sem_destroy( &shared.empty ); // contatore

    printf( "\n" );
    exit( EXIT_SUCCESS );
}