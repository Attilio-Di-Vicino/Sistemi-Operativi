/**
 * ESERCIZIO: 1
 *
 * Facendo uso della libreria Pthread( fatto con i semafori per esercizio ),
 *
 * realizzare un programma in cui un thread scrittore,
 * dato un intero N da riga di comando ( dove 10 < N <= 15 ),
 * scrive in un file nella prima posizione,
 *
 * uno alla volta ed ogni ½ secondo, la sequenza di Fibonacci di ordine N,
 * alternandosi con un thread lettore che legge, uno alla volta dalla
 * prima posizione del file, i numeri scritti dal thread scrittore.
 *
 * Un terzo thread attende la lettura dell’ N-esimo intero, quindi
 * stampa a video il messaggio
 * “Operazioni concluse, arrivederci dal thread: tid”,
 * attende 5 secondi e termina.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>

#define TIMESLEEP 1
#define SIZEFILE 255
#define OVER -1

// Semafori
struct sync {
    sem_t full;
    sem_t empty;
    sem_t sem_CS;
    sem_t print;
} shared;

// Variabili globali
int file;
int lettore;
int N;
char* fileName;

/**
 * Utilizzo la tecnica della memoization
 * per la risoluzione di fibonacci ( ASD )
*/
int* memoization;

int fibonacci( int );
void* writer( void* );
void* reader( void* );
void* final( void* );
int main( int argc, char* argv[] ) {

    // Primo controllo verifico i parametri in input
    if ( argc != 3 ) {
        printf( "\nusage: %s <filename> <#N>", argv[0] );
        exit( EXIT_SUCCESS );
    }

    // Assegno il nome del file ed il valore ad N globale con cast
    fileName = argv[1];
    N = atoi( argv[2] );

    // Allocaione e inizializzazione memoization
    memoization = ( int* ) calloc( N + 1, sizeof( int ) );
    for ( int i = 0; i <= N; i++ )
        memoization[i] = OVER;

    // Verifico che N rispetti il valore
    if ( N <= 10 || N > 15 ) {
        printf( "\nInserisci 10 < N <= 15" );
        exit( EXIT_SUCCESS );
    }

    // Apertura file
    file = open( fileName, O_RDWR | O_CREAT | O_TRUNC, 0777 );
    lettore = file;

    // Controllo apetura file corretta
    if ( file == -1 ) {
        perror( "Errore apertura file" );
        exit( EXIT_FAILURE );
    }

    printf( "\nFile: %s\nN: %d -> Fibonacci: %d", fileName, N, fibonacci( N ) );

    // Inizializzazione semafori
    sem_init( &shared.full, 0, 0 );
    sem_init( &shared.empty, 0, N );
    sem_init( &shared.sem_CS, 0, 1 );
    sem_init( &shared.print, 0, 0 );

    // Thread
    pthread_t write, read, last;

    // Creazione Thread
    if ( pthread_create( &write, NULL, writer, NULL ) != 0 ) {
        perror( "Errore nella creazione del thread scrittore" );
        exit( EXIT_FAILURE );
    }
    if ( pthread_create( &read, NULL, reader, NULL ) != 0 ) {
        perror( "Errore nella creazione del thread lettore" );
        exit( EXIT_FAILURE );
    }
    if ( pthread_create( &last, NULL, final, NULL ) != 0 ) {
        perror( "Errore nella creazione del thread finale" );
        exit( EXIT_FAILURE );
    }

    // Attesa Thread
    if ( pthread_join( write, NULL ) != 0 ) {
        perror( "Errore nella join del thread scrittore" );
        exit( EXIT_FAILURE );
    }
    if ( pthread_join( read, NULL ) != 0 ) {
        perror( "Errore nella join del thread lettore" );
        exit( EXIT_FAILURE );
    }
    if ( pthread_join( last, NULL ) != 0 ) {
        perror( "Errore nella join del thread finale" );
        exit( EXIT_FAILURE );
    }

    // Chiusura file
    close( file );

    // deallocazione
    sem_destroy( &shared.full );
    sem_destroy( &shared.empty );
    sem_destroy( &shared.sem_CS );
    sem_destroy( &shared.print );

    printf( "\n" );
    exit( EXIT_SUCCESS );
}

int fibonacci( int n ) {
    if ( memoization[n] != OVER )
        return memoization[n];
    if ( n <= 1 )
        memoization[n] = n;
    else 
        memoization[n] = fibonacci( n - 1 ) + fibonacci( n - 2 );
    return memoization[n];
}

void* writer( void* arg ) {

    for ( int i = 0; i <= N; i++ ) {
        sleep( TIMESLEEP );
        sem_wait( &shared.empty );
        sem_wait( &shared.sem_CS );
        // Sezione critica
        // Scrivo sul file
        int fibo = memoization[i];
        lseek( file, 0, SEEK_SET);
        write( file, &fibo, sizeof( int ) );
        printf( "\nScrittore: %d", fibo );
        sem_post( &shared.sem_CS );
        sem_post( &shared.full );
    }
    pthread_exit( NULL );
}

void* reader( void* arg ) {

    int number;

    for ( int i = 0; i <= N; i++ ) {
        sleep( TIMESLEEP );
        sem_wait( &shared.full );
        sem_wait( &shared.sem_CS );
        // Sezione critica
        // Leggo sul file
        lseek( lettore, 0, SEEK_SET) ;
        read( lettore, &number, sizeof( int ) );
        printf( "\nLettore: %d", number );
        sem_post( &shared.sem_CS );
        sem_post( &shared.empty );
    }
    sem_post( &shared.print );
    pthread_exit( NULL );
}

void* final( void* arg ) {

    sem_wait( &shared.print );
    printf( "\nOperazioni concluse, arrivederci dal thread: %ld", pthread_self() );
    sleep( 5 );
    pthread_exit( NULL );
}