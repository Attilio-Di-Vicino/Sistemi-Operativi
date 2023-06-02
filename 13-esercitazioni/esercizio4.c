/**
 * ESERCIZIO: 4
 * 
 * Scrivere un programma C e Posix sotto Linux che,
 * utilizzando la libreria Pthread crei due Thread, chiamati produttore e consumatore.
 * La risorsa condivisa, è un buffer circolare di dimensione data
 * ( ad esempio 20 ) il cui stato è:
 * 
 * • numero di elemento contenuti: count
 * • puntatore alla prima posizione libera: writepos
 * • puntatore al primo elemento occupato: readpos
 * 
 * Il produttore, inserisce, 20 numeri interi in maniera sequenziale.
 * 
 * Il consumatore li estrae sequenzialmente per stamparli.
 * 
 * Il programma dovrà prevedere:
 * 
 * • un meccanismo di accesso controllato alla risorsa buffer da parte dei due Thread
 *  ( mutex per il controllo della mutua esclusione nell'accesso al buffer )
 * 
 * • una sincronizzazione tra il produttore ed il consumatore ( Thread ) in caso di
 * 
 *      -> buffer pieno: definizione di una condition per la sospensione del 
 *         produttore se il buffer è pieno ( notfull )
 * 
 *      -> buffer vuoto: definizione di una condition per la sospensione del
 *         consumatore se il buffer è vuoto ( notempty )
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

// Direttive al processore
#define SIZE 20 // Size buffer
#define MAXVAL 100 // Valore massimo che si può produrre
#define MAX_ITER 46 // Limite iterazioni
#define OVER -1 // Valore nullo all'interno del buffer

// Struttura condivisa
struct buffer {
    int count; // contatore elementi contenuti
    int writepos; // prima posizione disponibile
    int readpos; // posizione primo elemento disponibile
    int* myBuffer;
};

void printBuffer( struct buffer* myBuffer ) {
    printf( "\nNel buffer ci sono %d elementi", myBuffer->count );
    printf( "\nLa prima posizione disponibile è %d", myBuffer->writepos % SIZE );
    printf( "\nIl primo elemento disponibile si trova nella posizione %d", myBuffer->readpos % SIZE );
}

void printMyBuffer( struct buffer* sharedBuffer ) {
    printf( "\n\nBuffer: " );
    for ( int i = 0; i < SIZE; i++ )
        printf( "%d ", sharedBuffer->myBuffer[i] );
    printf( "\n" );
}

// Mutex e Var cond inizializzate staticamente
struct sync {
    pthread_mutex_t mutex;
    pthread_cond_t notfull;
    pthread_cond_t notempty;
} shared = { PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER, PTHREAD_COND_INITIALIZER };

/**
 * routine del Thread produttore
 * essa produce ed inserisce il valore prodotto
 * nel buffer condiviso, ed aggiorna i puntatori
 * in mutua esclusione, e sincronizzato col Thread consumatore
 * 
 * @param argv struttura condivisa
*/
void* produce( void* argv ) {
    struct buffer* sharedBuffer = ( struct buffer* ) argv;

    // Impongo un limite alle iterazioni
    for ( int i = 0; i < MAX_ITER; i++ ) {
        while ( sharedBuffer->count >= SIZE )
            pthread_cond_wait( &shared.notfull, &shared.mutex );
        // Sezione critica
        int val = rand() % MAXVAL + 1;
        sharedBuffer->myBuffer[ sharedBuffer->writepos % SIZE ] = val;
        printf( "\nThread produttore produce il valore [%d] alla posizione [%d]", val, sharedBuffer->writepos % SIZE );
        sharedBuffer->writepos++;
        sharedBuffer->count++;
        pthread_cond_signal( &shared.notempty );
        pthread_mutex_unlock( &shared.mutex );
    }
    pthread_exit( EXIT_SUCCESS );
}

/**
 * routine del Thread consumatore
 * essa consuma e svuota il valore consumato alla rispettiva posizione
 * nel buffer condiviso, ed aggiorna i puntatori
 * in mutua esclusione, e sincronizzato col Thread produttore
 * 
 * @param argv struttura condivisa
*/
void* consume( void* argv ) {
    struct buffer* sharedBuffer = ( struct buffer* ) argv;

    // Impongo un limite alle iterazioni
    for ( int i = 0; i < MAX_ITER; i++ ) {
        while ( sharedBuffer->count == 0 )
            pthread_cond_wait( &shared.notempty, &shared.mutex );
        // Sezione critica
        int val = sharedBuffer->myBuffer[ sharedBuffer->readpos % SIZE ];
        sharedBuffer->myBuffer[ sharedBuffer->readpos % SIZE ] = - 1;
        printf( "\nThread consumatore consuma il valore [%d] alla posizione [%d]", val, sharedBuffer->readpos % SIZE );
        sharedBuffer->readpos++;
        sharedBuffer->count--;
        pthread_cond_signal( &shared.notfull );
        pthread_mutex_unlock( &shared.mutex );
    }
    pthread_exit( EXIT_SUCCESS );
}

void main() {

    // Allocazione e inizializzazione buffer condiviso
    struct buffer* sharedBuffer = ( struct buffer* ) malloc( sizeof( struct buffer ) );
    sharedBuffer->count = 0;
    sharedBuffer->readpos = 0;
    sharedBuffer->writepos = 0;
    sharedBuffer->myBuffer = ( int* ) calloc( SIZE, sizeof( int ) );
    for ( int i = 0; i < SIZE; i++ )
        sharedBuffer->myBuffer[i] = OVER;

    // Thread
    pthread_t produttore, consumatore;

    // Creazione Thread produttore
    if ( pthread_create( &produttore, NULL, produce, ( void* ) sharedBuffer ) != 0 ) {
        perror( "Errore nella creazione del Thread Produttore" );
        exit( EXIT_FAILURE );
    }

    // Creazione Thread consumatore
    if ( pthread_create( &consumatore, NULL, consume, ( void* ) sharedBuffer ) != 0 ) {
        perror( "Errore nella creazione del Thread Consumatore" );
        exit( EXIT_FAILURE );
    }

    // Attesa Thread produttore
    if ( pthread_join( produttore, NULL ) != 0 ) {
        perror( "Errore nella join del Thread Produttore" );
        exit( EXIT_FAILURE );
    }

    // Attesa Thread consumatore
    if ( pthread_join( consumatore, NULL ) != 0 ) {
        perror( "Errore nella join del Thread Consumatore" );
        exit( EXIT_FAILURE );
    }

    printMyBuffer( sharedBuffer );
    printBuffer( sharedBuffer );
    printf( "\n" );
    exit( EXIT_SUCCESS );
}