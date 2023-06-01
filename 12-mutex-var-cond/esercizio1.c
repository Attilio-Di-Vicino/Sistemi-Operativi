/**
 * ESERCIZIO: 1
 * 
 * Si realizzi un programma in C e Posix sotto Linux 
 * che, utilizzando la libreria Pthread:
 * 
 * • lancia n thread per cercare un elemento in una matrice nxn di interi
 * 
 * • Ognuno dei thread cerca l'elemento in una delle righe della matrice
 * 
 * • Non appena un thread ha trovato l'elemento cercato, 
 *   rende note agli altri thread le coordinate dell'elemento
 *   e tutti i thread terminano (sono cancellati)
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

// Variabili globali
int** matrix;
int sizeMatrix;
int findNumber;
int row = - 1, col = - 1;

// Mutex
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void initMatrix() {
    for ( int i = 0; i < sizeMatrix; i++ )
        for ( int j = 0; j < sizeMatrix; j++ )
            matrix[i][j] = rand() % 10;
}

void printMatrix() {
    printf( "\n" );
    for ( int i = 0; i < sizeMatrix; i++ ) {
        for ( int j = 0; j < sizeMatrix; j++ )
            printf( "%d    ", matrix[i][j] );
        printf( "\n" );
    }
}

void allocationMatrix() {
    matrix = ( int** ) calloc( sizeMatrix, sizeof( int* ) );
    for ( int i = 0; i < sizeMatrix; i++ )
        matrix[i] = ( int* ) calloc( sizeMatrix, sizeof( int ) );
}

void deAllocationMatrix() {
    for ( int i = 0; i < sizeMatrix; i++ )
        free( matrix[i] );
    free( matrix );
}

void* routine( void* argv ) {
    pthread_t* myTid = ( pthread_t* ) argv;
    printf( "\nMy Tid is: %ld", *myTid );

    for ( int j = 0; j < sizeMatrix; j++ ) {
        if ( matrix[ *myTid ][j] ==  findNumber ) {
            pthread_mutex_lock( &mutex );
            row = *myTid;
            col = j;
            pthread_mutex_unlock( &mutex );

            for ( int i = 0; i < sizeMatrix; i++ ) {
                if ( i != *myTid ) {
                    // pthread_cancel( i );
                }
            }
        }
    }
}

int main( int argc, char* argv[] ) {

    // Prendo come seme l'orario attuale
    srand( time( NULL ) );

    if ( argc != 2 ) {
        perror( "Errore nei parametri in input" );
        exit( EXIT_FAILURE );
    }

    sizeMatrix = atoi( argv[1] );
    printf( "\nMatrix sarà di %d righe ed %d colonne", sizeMatrix, sizeMatrix );

    // Allocazione, inizializzazione ed stampa Matrix
    allocationMatrix();
    initMatrix();
    printMatrix();

    printf( "Inserisci il numero da cercare: " );
    scanf( "%d", &findNumber );

    // Thread
    pthread_t th[ sizeMatrix ];

    // Creazione Thread
    for ( int i = 0; i < sizeMatrix; i++ ) {
        int* tid = malloc( sizeof( int* ) );
        *tid = i;
        if ( pthread_create( &th[i], NULL, routine, ( void* ) tid ) != 0 ) {
            perror( "Errore nella creazione dei Thread" );
            exit( EXIT_FAILURE );
        }
    }

    // Attesa Thread
    for ( int i = 0; i < sizeMatrix; i++ ) {
        if ( pthread_join( th[i], NULL ) != 0 ) {
            perror( "Errore nella join dei Thread" );
            exit( EXIT_FAILURE );
        }
    }

    if ( row != - 1 && col != -1 )
        printf( "\nRiga: %d, Colonna: %d", row, col);
    else 
        printf( "\nNumero non trovato!" );

    // Deallocazione ed uscita dal programma
    deAllocationMatrix();
    printf( "\n" );
    exit( EXIT_SUCCESS );
}