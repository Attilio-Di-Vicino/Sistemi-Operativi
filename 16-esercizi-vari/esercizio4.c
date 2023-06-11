/**
 * ESERCIZIO: 4
 * 
 * Si realizzi un programma C e Posix in ambiente Linux che, impiegando la libreria Pthread,  
 * crei una matrice di interi di dimensioni nxn con n numero dispari fornito da riga di comando,  
 * che provveda a sommare in modo concorrente con quattro thread: 
 * 
 *  1. gli elementi delle due diagonali
 *  2. gli elementi della riga centrale
 *  3. gli elementi della colonna centrale
 * 
 * e ne determini il massimo da assegnare ad un'opportuna variabile.
 * utilizzare semafori basati su memoria
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define MAXVALUE 9
#define CENTRAL N / 2

// Variabili globali
int** matrix;
int N;
int count;
int max;

// Struttura globale
struct sync {
	sem_t sem_CS;
} shared;

void allocationMatrix() {
	matrix = ( int** ) calloc( N, sizeof( int* ) );
	for ( int i = 0; i < N; i++ )
		matrix[i] = ( int* ) calloc( N, sizeof( int ) );
}

void initMatrix() {
	for ( int i = 0; i < N; i++ )
		for ( int j = 0; j < N; j++ )
			matrix[i][j] = rand() % MAXVALUE + 1;
}

void printMatrix() {
	printf( "\nMatrix:\n" );
	for ( int i = 0; i < N; i++ ) {
		for ( int j = 0; j < N; j++ )
			printf( "%d	", matrix[i][j] );
		printf( "\n" );
	}
}

void deallocationMatrix() {
	for ( int i = 0; i < N; i++ )
		free( matrix[i] );
	free( matrix );
}

void verify( int result ) {
	sem_wait( &shared.sem_CS );
	if ( result > max )
		max = result;
	if ( ++count == 4 )
		printf( "\nIl massimo è: %d", max );
	sem_post( &shared.sem_CS );
}

void* diagonalSX( void* arg ) {
	printf( "\nI'm the thread sx" );
	int result;
	for ( int i = 0; i < N; i++ ) {
		result += matrix[i][i];
	}
	printf( "\nresult sx: %d", result );
	verify( result );
	pthread_exit( NULL );
}

void* diagonalDX( void* arg ) {
	printf( "\nI'm the thread dx" );
	int result;
	for ( int i = 0; i < N; i++ ) {
		result += matrix[i][ N - i - 1 ];
	}
	printf( "\nresult dx: %d", result );
	verify( result );
	pthread_exit( NULL );
}

void* computeRow( void* arg ) {
	printf( "\nI'm the thread row" );
	int result;
	for ( int i = 0; i < N; i++ ) {
		result += matrix[ CENTRAL ][i];
	}
	printf( "\nresult row: %d", result );
	verify( result );
	pthread_exit( NULL );
}

void* computeCol( void* arg ) {
	printf( "\nI'm the thread col" );
	int result;
	for ( int i = 0; i < N; i++ ) {
		result += matrix[i][ CENTRAL ];
	}
	printf( "\nresult col: %d", result );
	verify( result );
	pthread_exit( NULL );
}

void main( int argc, char* argv[] ) {
	
	// Controllo parametri in input da riga di comando
	if ( argc != 2 ) {
		printf( "\nusage: %s <#N>\n", argv[0] );
		exit( EXIT_FAILURE );
	}
	
	N = atoi( argv[1] );
	
	// Controllo N dispari
	if ( N % 2 == 0 ) {
		printf( "\nusage: %s <#N SHOTS>\n", argv[0] );
		exit( EXIT_FAILURE );
	}
	
	printf( "\nN: %d centrale: %d", N, CENTRAL );
	
	srand( time( NULL ) );
	
	// Matrix
	allocationMatrix();
	initMatrix();
	printMatrix();
	count = 0;
	max = 0;
	
	// Inizializzazione semafori binari
	if ( sem_init( &shared.sem_CS, 0, 1 ) != 0 ) {
		perror( "Errore nella creazione del semafor sem_CS" );
		exit( EXIT_FAILURE );
	}
	
	// Thread
	pthread_t sx, dx, row, col;
	
	// Creazione thread
	if ( pthread_create( &sx, NULL, diagonalSX, NULL ) != 0 ) {
		perror( "Errore nella creazione del thread sx" );
		exit( EXIT_FAILURE );
	}
	if ( pthread_create( &dx, NULL, diagonalDX, NULL ) != 0 ) {
		perror( "Errore nella creazione del thread dx" );
		exit( EXIT_FAILURE );
	}
	if ( pthread_create( &row, NULL, computeRow, NULL ) != 0 ) {
		perror( "Errore nella creazione del thread row" );
		exit( EXIT_FAILURE );
	}
	if ( pthread_create( &col, NULL, computeCol, NULL ) != 0 ) {
		perror( "Errore nella creazione del thread col" );
		exit( EXIT_FAILURE );
	}
	
	// Attesa thread
	if ( pthread_join( sx, NULL ) != 0 ) {
		perror( "Errore nella join del thread sx" );
		exit( EXIT_FAILURE );
	}
	if ( pthread_join( dx, NULL ) != 0 ) {
		perror( "Errore nella join del thread dx" );
		exit( EXIT_FAILURE );
	}
	if ( pthread_join( row, NULL ) != 0 ) {
		perror( "Errore nella join del thread row" );
		exit( EXIT_FAILURE );
	}
	if ( pthread_join( col, NULL ) != 0 ) {
		perror( "Errore nella join del thread col" );
		exit( EXIT_FAILURE );
	}
	
	
	// Deallocazione matrix
	deallocationMatrix();
	
	// Deallocazione semafori basati su memoria
	if ( sem_destroy( &shared.sem_CS ) != 0 ) {
		perror( "Errore nella destroy del semaforo sem_CS" );
		exit( EXIT_FAILURE );
	}
	
	printf( "\n" );
	exit( EXIT_SUCCESS );
}