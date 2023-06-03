/**
 * ESAME: Prova Scritta 21/01/2022
*/

/**
 * 9 punti ( Obbligatorio )
 * 
 * Data una matrice nxn ( n pari ) di interi generati casualmente, allocata dinamicamente, con n argomento da riga di comando,
 * creare n thread che prelevano casualmente un elemento dalla riga di competenza ( thread i-esimo, riga i-esima )
 * e lo inseriscano concorrentemente in un vettore di ( ( n + 1 ) / 2 ) elementi.
 * 
 * Un Thread n+1-iesimo attende il riempimento del vettore per stampare il contenuto dello stesso e per stampare
 * il numero di elementi inserito nel vettore da casciun thread.
 * 
 * Usare mutex e variabili condizione
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAXVALUE 9
#define OVER -1

// Variabili globali
int** matrix;
int N;
int* vectorResult;
int count = 0;

// Mutex e var cond inizializzate staticamente
struct sync {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} shared = { PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER };

void allocationMatrixVector() {
    // Matrix
    matrix = ( int** ) calloc( N, sizeof( int* ) );
    for ( int i = 0; i < N; i++ )
        matrix[i] = ( int* ) calloc( N, sizeof( int ) );
    // Vector
    vectorResult = ( int* ) calloc( ( N + 1 ) / 2, sizeof( int ) );
}

void initMatrixVector() {
    // Matrix
    for ( int i = 0; i < N; i++ )
        for ( int j = 0; j < N; j++ )
            matrix[i][j] = rand() % MAXVALUE + 1;
    // Vector
    for ( int i = 0; i < ( N + 1 ) / 2; i++ )
        vectorResult[i] = OVER;
}

void printMatrix() {
    printf( "\nMatrix:\n" );
    for ( int i = 0; i < N; i++ ) {
        for ( int j = 0; j < N; j++ )
            printf( "%d    ", matrix[i][j] );
        printf( "\n" );
    }
}

void printVector() {
    printf( "\nVector: " );
    for ( int i = 0; i < ( N + 1 ) / 2; i++ )
        printf( "%d  ", vectorResult[i] );
}

void deallocationMatrixVector() {
    // Matrix
    for ( int i = 0; i < N; i++ )
        free( matrix[i] );
    free( matrix );
    // Vector
    free( vectorResult );
}

void* slave( void* argv ) {
    pthread_t* myTid = ( pthread_t* ) argv;

    // Plevelo casualmente
    int val = OVER;
    int pos = rand() % N;
    val = matrix[ *myTid ][pos];

    // Inserisco il valore in maniera concorrente
    pthread_mutex_lock( &shared.mutex );
    // Controllo se ci sono ancora posti disponibili
    if ( count >= ( N + 1 ) / 2 ) {
        // Non ci sono più posti disponibili
        // Sblocco il mutex ed esco
        pthread_mutex_unlock( &shared.mutex );
        pthread_exit( EXIT_SUCCESS );
    }
    // Ci sono ancora posti disponibili nel vettore
    // Inserisco il valore ed incremento il puntatore count
    vectorResult[ count++ ] = val;
    // Se sono l'ultimo a dover inserire invio il segnale per
    // sbloccare il processo master
    if ( count == ( N + 1 ) / 2 )
        pthread_cond_signal( &shared.cond );
    pthread_mutex_unlock( &shared.mutex );

    pthread_exit( EXIT_SUCCESS );
}

void* master( void* argv ) {

    // Aquisisco il mutex
    pthread_mutex_lock( &shared.mutex );
    // Verifico se il vettore è completo
    // Quindi utilizzo la wait associata ad una variabile di condizione
    // ed a un mutex nel caso in cui non è ancora completo il vettore
    // rilascio il mutex ed aspetto finchè non mi risveglino
    while ( count < ( N + 1 ) / 2 )
        pthread_cond_wait( &shared.cond, &shared.mutex );
    // Rilascio il mutex
    pthread_mutex_unlock( &shared.mutex );

    // Stampo ed esco
    // il vettore non è più soggeto a race condiction
    // perchè i thread slave non andranno più a modificarlo
    printVector();
    pthread_exit( EXIT_SUCCESS );
}

void main( int argc, char* argv[] ) {

    // Controllo se il numero di paramenti in input da riga di comando sono corretti
    if ( argc != 2 ) {
        perror( "Errore nei paramenti in input da riga di comando" );
        exit( EXIT_FAILURE );
    }

    // Parametro da riga di comando
    N = atoi( argv[1] );
    printf( "\nIl valore inserito è: %d", N );

    // Allocazione, inizializzazione e stampa matrice
    allocationMatrixVector();
    initMatrixVector();
    printMatrix();
    printVector();

    // Thread
    pthread_t th[ N + 1 ];

    // Creazione Thread slave
    for ( int i = 0; i < N; i++ ) {
        int* tid = ( int* ) malloc( sizeof( int ) );
        *tid = i;
        if ( pthread_create( &th[i], NULL, slave, ( void* ) tid ) != 0 ) {
            perror( "Errore nella creazione dei thread slave" );
            exit( EXIT_FAILURE );
        }
    }

    // Creazione Thread Master
    if ( pthread_create( &th[ N ], NULL, master, ( void* ) NULL ) != 0 ) {
        perror( "Errore nella creazione del thread master" );
        exit( EXIT_FAILURE );
    }

    // Attesa Thread slave
    for ( int i = 0; i < N; i++ ) {
        if ( pthread_join( th[i], NULL ) != 0 ) {
            perror( "Errore nella join dei thread slave" );
            exit( EXIT_FAILURE );
        }
    }

    // Attesa Thread master
    if ( pthread_join( th[ N ], NULL ) != 0 ) {
        perror( "Errore nella join dei thread master" );
        exit( EXIT_FAILURE );
    }

    // Deallocazione e uscita del processo
    deallocationMatrixVector();
    printf( "\n" );
    exit( EXIT_SUCCESS );
}