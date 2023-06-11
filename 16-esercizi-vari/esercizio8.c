/**
 * ESERCIZIO: 8
 * 
 * Si progetti un programma in ambiente Linux che, impiegando  la  libreria  pthread,
 * crei  un  thread  che  estrae N volte ( con N passato da riga di comando )
 * un numero casuale compreso tra 0 e 10 e ne calcoli il fattoriale  ponendolo 
 * in  una  variabile  condivisa e  un  thread  che  stampi  a  video  il  fattoriale 
 * calcolato  N  volte prelevandolo dalla variabile condivisa.
 *
 * Per la sincronizzazione utilizzare i semafori con nome
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>

#define MAXVALUE 10
#define WRITER "/writer"
#define READER "/reader"
#define OFLAG O_CREAT | O_EXCL
#define FILE_MODE S_IRUSR | S_IWUSR
#define OVER -1

struct sync {
	sem_t* writer;
	sem_t* reader;
} shared;

// Variabili globali
int N;
int generatedNumber;
long int resultFactorial;
int* memoization; // sfrutto la programmazione dinamica

long int fattoriale( int );
void* slave( void* );
void* master( void* );
void main( int argc, char* argv[] ) {

	// Controlllo i parametri presi in input da riga di comando
	if ( argc != 2 ) {
		printf( "\nusage: %s <#N>\n", argv[0] );
		exit( EXIT_FAILURE );
	}
	
	N = atoi( argv[1] );
		
	// Stampa di verifica
	printf( "\nN: %d", N );
	
	srand( time( NULL ) );

    // Inizializzazione memoization
    memoization = ( int* ) malloc( N * sizeof( int ) );
    for ( int i = 0; i < N; i++ )
        memoization[i] = OVER;
	
	// unlink dei semafori
	if ( sem_unlink( WRITER ) != 0 ) {
        perror( "Errore nell'unlink del semaforo writer" );
        exit( EXIT_FAILURE );
    }
	if ( sem_unlink( READER ) != 0 ) {
        perror( "Errore nell'unlink del semaforo reader" );
        exit( EXIT_FAILURE );
    }
	
	// apertura semafori binari
	shared.writer = sem_open( WRITER, OFLAG, FILE_MODE, 1 );
	shared.reader = sem_open( READER, OFLAG, FILE_MODE, 0 );
	
	// Controllo dei semafori
	if ( shared.writer == NULL ) {
		perror( "Errore nella creazione del semafori writer" );
		exit( EXIT_FAILURE );
	}
    if ( shared.reader == NULL ) {
		perror( "Errore nella creazione del semafori reader" );
		exit( EXIT_FAILURE );
	}
	
	// Thread
	pthread_t thMaster, thSlave;
	
	// Creazione Thread
	if ( pthread_create( &thSlave, NULL, slave, NULL ) != 0 ) {
		perror( "Errore nella creazione del thread slave" );
		exit( EXIT_FAILURE );
	}
	if ( pthread_create( &thMaster, NULL, master, NULL ) != 0 ) {
		perror( "Errore nella creazione del thread master" );
		exit( EXIT_FAILURE );
	}
	
	// Attesa Thread
	if ( pthread_join( thSlave, NULL ) != 0 ) {
		perror( "Errore nella join del thread slave" );
		exit( EXIT_FAILURE );
	}
	if ( pthread_join( thMaster, NULL ) != 0 ) {
		perror( "Errore nella join del thread master" );
		exit( EXIT_FAILURE );
	}
	
	// chiusura semafori
	if ( sem_close( shared.writer ) != 0 ) {
        perror( "Errore nella close del semaforo writer" );
        exit( EXIT_FAILURE );
    }
	if ( sem_close( shared.reader ) != 0 ) {
        perror( "Errore nella close del semaforo reader" );
        exit( EXIT_FAILURE );
    }
	
	// unlink dei semafori
	if ( sem_unlink( WRITER ) != 0 ) {
        perror( "Errore nell'unlink del semaforo writer" );
        exit( EXIT_FAILURE );
    }
	if ( sem_unlink( READER ) != 0 ) {
        perror( "Errore nell'unlink del semaforo reader" );
        exit( EXIT_FAILURE );
    }
	
	printf( "\n" );
	exit( EXIT_SUCCESS );
}

long int fattoriale( int n ) {
    if ( memoization[n] != OVER )
        return memoization[n];
	if ( n == 1 || n == 0 )
		memoization[n] = 1;
    else
        memoization[n] = n * fattoriale( n - 1 );
	return memoization[n];
}

void* slave( void* arg ) {
	for ( int i = 0; i < N; i++ ) {
		int rs = rand() % ( MAXVALUE + 1 );
		sem_wait( shared.writer );
		generatedNumber = rs;
		resultFactorial = fattoriale( generatedNumber );
		sem_post( shared.reader );
	}
	pthread_exit( NULL );
}

void* master( void* arg ) {
	for ( int i = 0; i < N; i++ ) {
		sem_wait( shared.reader );
		printf( "\n%d. fattoriale di %d -> %ld", i+1, generatedNumber, resultFactorial );
		sem_post( shared.writer );
	}
	pthread_exit( NULL );
}