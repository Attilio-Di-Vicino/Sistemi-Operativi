/**
 * ESERCIZO: 2
 * 
 * • Dato un ponte con una capacità massima MAX che esprime il numero 
 *   massimo di veicoli che possono transitare contemporaneamente su di esso.
 *   
 *   Considerata l’ipotesi che sul ponte possono transitare solo due 
 *   tipi di veicoli: automobili e camion e che il ponte è talmente stretto 
 *   che il transito di un camion in una particolare direzione impedisce 
 *   l'accesso al ponte di altri veicoli in direzione opposta.
 * 
 * • Realizzare una politica di sincronizzazione delle entrate e delle uscite 
 *   dal ponte che tenga conto delle specifiche date e che favorisca le
 *   automobili rispetto ai camion nell'accesso al ponte.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <fcntl.h>

#define MAX 20
#define CATEGORY 2
#define CORSIE 2
#define FALSE 0
#define TRUE 1
#define OFLAG O_CREAT | O_EXCL
#define FILEMODE S_IRUSR | S_IWUSR

// Struttura condivisa che comprende
// Semafori per sincronizzare l'accesso al ponte
// Ed mutex e variabili condizione
struct sync {
	sem_t* empty;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
} shared;

// predicato globale
int passaggioCamion[ CORSIE ] = { FALSE, FALSE };

void* automobile( void* arg ) {

	pthread_t* myTid = ( pthread_t* ) arg;
	
	// indica la corsia che dovrò percorrere
	int rs = rand() % CORSIE;
	int rsReverse;
	
	// inverto la corsia perchè la corsia 
	// che dovrò controllare sarà l'opposta
	if ( rs == 0 )
		rsReverse = 1;
	else 
		rsReverse = 0;
	
    // Attendo nel caso ci siano MAX veicoli sul ponte
    // implementando così la concorrenza limitata
	sem_wait( shared.empty );
    // Controllo che nella corsia opposta non stiano venendo camion
    // il tutto in mutua esclusione
	pthread_mutex_lock( &shared.mutex );
	while( passaggioCamion[ rsReverse ] == TRUE )
		pthread_cond_wait( &shared.cond, &shared.mutex );
	pthread_mutex_unlock( &shared.mutex );
    // la corsia opposta è libera da camion e posso passare
	printf( "\nAutomobile %ld è passato sulla corsia %d", *myTid, rs );
    // esco e libero un posto sul ponte
	sem_post( shared.empty );
}

void* camion( void* arg ) {

	pthread_t* myTid = ( pthread_t* ) arg;
	
	// indica la corsia che dovrò percorrere
	int rs = rand() % CORSIE;
	int rsReverse;
	
	// inverto la corsia perchè la corsia 
	// che dovrò controllare sarà l'opposta
	if ( rs == 0 )
		rsReverse = 1;
	else 
		rsReverse = 0;
	// Attendo nel caso ci siano MAX veicoli sul ponte
    // implementando così la concorrenza limitata
	sem_wait( shared.empty );
    // Controllo che nella corsia opposta non stiano venendo camion
    // il tutto in mutua esclusione
	pthread_mutex_lock( &shared.mutex );
	while( passaggioCamion[ rsReverse ] == TRUE )
		pthread_cond_wait( &shared.cond, &shared.mutex );
    // essendo un camion avviso che sto passando
	passaggioCamion[ rs ] = TRUE;
    pthread_mutex_unlock( &shared.mutex );
    // passo
	printf( "\nCamion %ld è passato sulla corsia %d", *myTid, rs );
    // rientro in mutua esclusione
    pthread_mutex_lock( &shared.mutex );
    // una volta passato avviso che sono uscito dal ponte
	passaggioCamion[ rs ] = FALSE;
	pthread_cond_signal( &shared.cond );
	pthread_mutex_unlock( &shared.mutex );
    // esco e libero un posto sul ponte
	sem_post( shared.empty );
}

void main( int argc, char* argv[] ) {

	printf( "\nMax: %d", ( int ) MAX );
	
	// Eseguiamo l'unlink
	sem_unlink( "empty" );
	
	// Apriamo i semafori
	shared.empty = sem_open( "empty", OFLAG, FILEMODE, MAX );
	
	// Inizializzazione dinamica di mutex e variabile condizione
	pthread_mutex_init( &shared.mutex, NULL );
	pthread_cond_init( &shared.cond, NULL );
	
	// Thread
	pthread_t th[ MAX * 2 ];
	
    // Creazione randomatica dei thread 
	for ( int i = 0; i < MAX * 2; i++ ) {
		int rs = rand() % CATEGORY;
		int* tid = ( int* ) malloc( sizeof( int ) );
		*tid = i;
		if ( rs == FALSE ) {
			if ( pthread_create( &th[i], NULL, automobile, ( void* ) tid ) != 0 ) {
				perror( "Errore nella creazione di thread automobile" );
				exit( EXIT_FAILURE );
			}
		} else {
			if ( pthread_create( &th[i], NULL, camion, ( void* ) tid ) != 0 ) {
				perror( "Errore nella creazione di thread camion" );
				exit( EXIT_FAILURE );
			}
		}
	}
	
	// Attesa fine Thread
	for ( int i = 0; i < MAX * 2; i++ ) {
		if ( pthread_join( th[i], NULL ) != 0 ) {
			perror( "Errore nella join" );
			exit( EXIT_FAILURE );
		}
	}
	
	// Ciusura semafori
	sem_close( shared.empty );
	
	// Eseguiamo l'unlink
	sem_unlink( "empty" );
	
	// Deallozazione di mutex e variabile condizione
	pthread_mutex_destroy( &shared.mutex );
	pthread_cond_destroy( &shared.cond );
	
	printf( "\n" );
	exit( EXIT_SUCCESS );
}