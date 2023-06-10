/**
 * ESERCIZIO: #byAttilio
 * 
 * Si realizzi un programma C e Posix in ambiente Linux che, impiegando la libreria Pthread,
 * 
 * • lancia n thread per cercare un elemento in una matrice NxM di interi
 * 
 * • Ognuno dei thread cerca l'elemento in una delle righe della matrice
 * 
 * • Non appena un thread ha trovato l'elemento cercato, 
 *   rende note agli altri thread tramite un segnale SIGUSR1 l'avvenuto
 *   e tutti i thread terminano stampando un messaggio dedicato
 * 
 * • Infine ci sarà un N+1-esimo thread master il quale attende
 *   il risultato della ricerca e ne stampi il valore con un messaggio dedicato
 * 
 * • N ed M sono prese da riga di comando
 * • Il numero da cercare viene preso in input dallo standard io
 * • ogni thread slave aspetta N - tid secondi
 * • Utilizzare mutex e variabili condizione allocati staticamente
 * • la matrice deve essere allocata dinamicamente
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>

#define MAXVALUE 9
#define OVER -1
#define TIMESLEEP(tid) ( N - tid )

// Variabili globali
pthread_t* thSlave;
int N, M;
int research;
int** matrix;
int resultRow;
int resultCol;
int count;
int segnale;

// Struttura globale
struct sync {
	pthread_mutex_t mutex;
	pthread_cond_t cond;
} shared = { PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER };

void allocationMatrix() {
	matrix = ( int** ) calloc( N, sizeof( int* ) );
	for ( int i = 0; i < N; i++ )
		matrix[i] = ( int* ) calloc( M, sizeof( int ) );
}

void initMatrix() {
	for ( int i = 0; i < N; i++ )
		for ( int j = 0; j < M; j++ )
			matrix[i][j] = rand() % MAXVALUE + 1;
}

void printMatrix() {
	printf( "\nMatrix:\n" );
	for ( int i = 0; i < N; i++ ) {
		for ( int j = 0; j < M; j++ )
			printf( "%d	", matrix[i][j] );
		printf( "\n" );
	}
}

void* slave( void* arg ) {
	pthread_t* myTid = ( pthread_t* ) arg;
	printf( "\nHi guys, i'm the thread slave #%ld, attendo %ld sec..", *myTid, TIMESLEEP( *myTid ) );
	
	sleep( TIMESLEEP( *myTid ) );
	
	// Per tutte le colonne
	for ( int j = 0; j < M; j++ ) {
		// Il primo controllo che effettuo e se mi è 
		// stato inviato il segnale in tal caso
		// stampo un messaggio ed esco
		if ( segnale == SIGUSR1 ) {
			// Nel caso in cui il numero è gia stato trovato
			printf( "\nThread #%ld riceve il segnale, termina senza successo :(", *myTid );
			// Esco
			pthread_exit( NULL );
		}
		// Cerco il numero NB: la ricerca non è soggetta a race condiction			
		if ( matrix[ *myTid ][j] == research ) {
			// Trovato il numero vado in sezione critica
			pthread_mutex_lock( &shared.mutex );
			// Controllo prima che non sia stato gia trovato
			if ( resultRow == OVER && resultCol == OVER ) {
				// Numero trovato segno le coordinate
				resultRow = *myTid;
				resultCol = j;
				// Invio il segnale SIGUSR1 ai thread
				for ( int i = 0; i < N; i++ )
					pthread_kill( thSlave[i], SIGUSR1 );
				// Invio il segnale al thread master
				pthread_cond_signal( &shared.cond );
				// Fine sezione critica
				pthread_mutex_unlock( &shared.mutex );
				printf( "\nThread #%ld termina con successo :)", *myTid );
				// Esco
				pthread_exit( NULL );
			} else {
				// Fine sezione critica
				pthread_mutex_unlock( &shared.mutex );
			}
		}
	} 
	// Nel caso in cui il numero non esiste dobbiamo comunque
	// avvisare il thread master quindi utilizziamo la variabile count
	pthread_mutex_lock( &shared.mutex );
	if ( ++count == N )
		pthread_cond_signal( &shared.cond );
	printf( "\nThread #%ld termina senza successo :(", *myTid );
	// Fine sezione critica
	pthread_mutex_unlock( &shared.mutex );
	// Esco
	pthread_exit( NULL );
}

void* master( void* arg ) {
	printf( "\nHello Bombers, i'm the thread master.." );
	
	// Attendo il segnale effettuando
	// il controllo del predicato in mutua esclusione
	pthread_mutex_lock( &shared.mutex );
	while ( resultRow == OVER && resultCol == OVER && count < N )
		pthread_cond_wait( &shared.cond, &shared.mutex );
	pthread_mutex_unlock( &shared.mutex );
	// Stampa liberamente
	// NB: a questo punto resultRow e resultCol
	//     non sono più soggette a race condiction
	if ( resultRow != OVER && resultCol != OVER )
		printf( "\nElemento trovato -> matrix[%d][%d]: %d", resultRow, resultCol, matrix[ resultRow ][ resultCol ] );
	else
		printf( "\nElemento non trovato" );
	// Esco
	pthread_exit( NULL );
}

/**
 * Il gestore del segnale viene prima
 * agganciato utilizzando signal
 * quindi una volta che viene inviato
 * il segnale agganciato, allora verranno
 * eseguite le seguenti istuzioni di codice
 *
 * @param signo segnale ricevuto
*/
static void handler( int signo ) {
	segnale = signo;
}

void main( int argc, char* argv[] ) {
	
	// Primo controllo per i parametri in input da riga di comando
	if ( argc != 3 ) {
		printf( "\nusage: %s <#N> <#M>\n", argv[0] );
		exit( EXIT_FAILURE );
	}
	
	srand( time( NULL ) );
		
	N = atoi( argv[1] );
	M = atoi( argv[2] );
	
	printf( "\nI numeri inseriti sono -> N: %d, M: %d", N, M );
	
	// Allocazione, inizializzazione e stampa della matrice condivisa	
	allocationMatrix();
	initMatrix();
	printMatrix();
	
	printf( "\nOra inserisci il numero da ricercare: " );
	scanf( "%d", &research );
	
	// Inizializzazione variabili globali
	resultRow = OVER;
	resultCol = OVER;
	count = 0;
	
	// Aggancio il segnale
	if ( signal( SIGUSR1, handler ) == SIG_ERR ) {
		perror( "Errore nella cattura del segnale" );
		exit( EXIT_FAILURE );
	}
	
	// Thread
	thSlave = ( pthread_t* ) malloc( N * sizeof( pthread_t ) );
	pthread_t thMaster;
	
	// Creazione thread slave e master
	for ( int i = 0; i < N; i++ ) {
		int* tid = ( int* ) malloc( sizeof( int ) );
		*tid = i;
		if ( pthread_create( &thSlave[i], NULL, slave, ( void* ) tid ) != 0 ) {
			perror( "Errore nella creazione dei thread slave" );
			exit( EXIT_FAILURE );
		}
	}
	
	if ( pthread_create( &thMaster, NULL, master, NULL ) != 0 ) {
		perror( "Errore nella creazione dei thread master" );
		exit( EXIT_FAILURE );
	}
	
	// Attesa thread slave e master
	for ( int i = 0; i < N; i++ ) {
		if ( pthread_join( thSlave[i], NULL ) != 0 ) {
			perror( "Errore nella join dei thread slave" );
			exit( EXIT_FAILURE );
		}
	}
	
	if ( pthread_join( thMaster, NULL ) != 0 ) {
		perror( "Errore nella join dei thread master" );
		exit( EXIT_FAILURE );
	}
	
	printf( "\n" );
	exit( EXIT_SUCCESS );
}