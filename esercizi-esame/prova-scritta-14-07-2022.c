/**
 * ESAME: Prova Scritta 14/07/2022
*/

/**
 * 9 punti
 * 
 * Si realizzi un programma in C e Posix sotto linux che, con l'ausilio della libreria Pthread,
 * lancia m thread per calcolare la somma dei prodotti delle colonne di due matrici mxm.
 * 
 * L'i-esimo thread, dopo aver calcolato la somma dei prodotti delle colonne i-esime delle due matrici,
 * inserisce il risultato in un array m-dimensionale, in modo concorrente, nella prima locazione libera disponibile.
 * 
 * Non appena l'array sarà riempito, un m+1-esimo thread, che era in attesa, provvederà a stamparne il contenuto.
 * 
 * Le matrici devono essere allocate dinamicamente.
 * 
 * Usare mutex e variabili di condizione.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define M 8
#define MAXVALUE 9

// Variabili globali
int** matrixOne;
int** matrixTwo;

// Struttura condivisa
struct parameter {
    int* arrayResult; // array m-dimensionale contente i risultati
    int cont; // prima posizione disponibile
} parameters = { NULL, 0 };

// Struttura di mutex e var condizione 
// Globale ed inizializzata staticamente
struct sync {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} shared = { PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER };

void allocationMatrix() {
    matrixOne = ( int** ) calloc( M, sizeof( int* ) );
    matrixTwo = ( int** ) calloc( M, sizeof( int* ) );
    for ( int i = 0; i < M; i++ ) {
        matrixOne[i] = ( int* ) calloc( M, sizeof( int ) );
        matrixTwo[i] = ( int* ) calloc( M, sizeof( int ) );
    }
}

void initMatrix() {
    for ( int i = 0; i < M; i++ ) {
        for ( int j = 0; j < M; j++ ) {
            matrixOne[i][j] = rand() % MAXVALUE + 1;
            matrixTwo[i][j] = rand() % MAXVALUE + 1;
        }
    }
}

void printMatrix() {
    printf( "\nMatrix One:\n" );
    for ( int i = 0; i < M; i++ ) {
        for ( int j = 0; j < M; j++ )
            printf( "%d    ", matrixOne[i][j] );
        printf( "\n" );
    }
    printf( "\nMatrix Two:\n" );
    for ( int i = 0; i < M; i++ ) {
        for ( int j = 0; j < M; j++ )
            printf( "%d    ", matrixTwo[i][j] );
        printf( "\n" );
    }
}

void deallocationMatrix() {
    for ( int i = 0; i < M; i++ ) {
        free( matrixOne[i] );
        free( matrixTwo[i] );
    }
    free( matrixOne );
    free( matrixTwo );
}

void printArrayResult() {
    printf( "\nArrayResult: " );
    for ( int i = 0; i < M; i++ )
        printf( "%d ", parameters.arrayResult[i] );
    printf( "\n" );
}

void* routine( void* argv ) {
    pthread_t* myTid = ( pthread_t* ) argv;
    printf( "\nMy tid è: %ld", *myTid );

    long int partialResult = 0;

    for ( int j = 0; j < M; j++ )
        partialResult += matrixOne[ *myTid ][j] * matrixTwo[ *myTid ][j];

    pthread_mutex_lock( &shared.mutex );
    // La traccia richiede l'uso di variabili condizione
    // while ( parameters.cont >= M )
    //     pthread_cond_wait( &shared.cond, &shared.mutex );
    // Sezione critica
    parameters.arrayResult[ parameters.cont++ ] = partialResult;
    // pthread_cond_signal( &shared.cond );
    pthread_mutex_unlock( &shared.mutex );
    pthread_exit( NULL );
}

void main() {

    // Come seme recupero l'orario attuale
    srand( time( NULL ) );

    // Allocazione e inizializzazioni matrici in maniera dinamica
    allocationMatrix();
    initMatrix();
    printMatrix();

    // Allocazione e inizializzazione array risultato
    parameters.arrayResult = ( int* ) calloc( M, sizeof( int ) );
    parameters.cont = 0; // <- operazione ridondante
    for ( int i = 0; i < M; i++ )
        parameters.arrayResult[i] = 0;
    printArrayResult();

    // Thread
    pthread_t th[M];

    // Creazione M Thread
    for ( int i = 0; i < M; i++ ) {
        int* tid = ( int* ) malloc( sizeof( int ) );
        *tid = i;
        if ( pthread_create( &th[i], NULL, routine, ( void* ) tid ) != 0 ) {
            perror( "Errore nella creazione dei Thread" );
            exit( EXIT_FAILURE );
        }
    }

    // Attesa M Thread
    for ( int i = 0; i < M; i++ ) {
        if ( pthread_join( th[i], NULL ) != 0 ) {
            perror( "Errore nella join dei Thread" );
            exit( EXIT_FAILURE );
        }
    }

    printf( "\nTutti i Thread sono terminati." );
    printArrayResult();
    deallocationMatrix();
    printf( "\n" );
    exit( EXIT_SUCCESS );
}