/**
 * ESERCIZIO:
 * 
 * • Scrivere un programma che prende in input un intero n, il nome di 
 *   un file di testo ed un carattere x
 * 
 * • Il programma ha il compito di contare le occorrenze 
 *   del carattere x nel file di testo
 * 
 * • Il programma esegue tale compito creando n thread, ognuno dei
 *   quali esamina una porzione diversa del file di testo
 * 
 * • ad esempio, se il file è lungo 1000 bytes ed n=4, il primo thread
 *   esaminerà i primi 250 bytes, il secondo thread esaminerà i 250 bytes
 *   successivi, e così via
 * 
 * NB: viene utilizzato per semplicità un file di esempio
 *     il quale è nominato fileEsempio.txt
 *     esso è composto da
 *          a: 6
 *          b: 8
 *          c: 12
 *          d: 16
 *          e: 20
 *          TOT: 60
*/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define SIZE_FILE_NAME 100

char x;

struct parameter {
    FILE* file;
    int bytes;
    int pFile;
    int tid;
};

void* routine( void* argv ) {

    // Come prima cosa biogna eseguire il cast da void* alla struttura
    struct parameter* parameters = ( struct parameter* ) argv;
    
    printf( "\nSono il Thread: %d Il mio desc è: %d", parameters->tid, parameters->pFile );

    // Posiziono il descrittore nella posizione di inzio lettura
    fseek( parameters->file, parameters->pFile, SEEK_SET );

    // Buffer che conterrà i caratteri letti
    char buffer[ parameters->bytes ];

    // Lettura del file
    fread( buffer, sizeof( char ), parameters->bytes, parameters->file );

    int count = 0;
    for ( int i = 0; i <= parameters->bytes; i++ ) {
        if ( buffer[i] ==  x ){
            count++;
        }
    }
    pthread_exit( ( void* ) count );
}

const unsigned long int getFileSizeBytes( FILE* file ) {

    if ( file == NULL ){
        perror( "Errore non si possono eseguire operazioni su un file chiuso NULL" );
        exit( EXIT_FAILURE );
    }

    unsigned long int bytes;

    // Sopstati alla fine del file
    if ( fseek( file, 0, SEEK_END ) != 0 ) {
        perror( "Errore nella fseek" );
        exit( EXIT_FAILURE );
    }

    // Recupero il descrittore del file,
    // che attualmente si trova alla fine del file
    bytes = ftell( file );

    // Riporto il descrittore all'inizio del file
    rewind( file );

    return bytes;
}

int main() {

    int n;
    char fileName[ SIZE_FILE_NAME ];

    printf( "\nInserisci un intero N: " );
    scanf( "%d", &n );

    printf( "\nInserisci il nome del file: " );
    scanf( "%s", fileName );

    /**
     * ATTENZIONE: 
     * 
     * Lo spazio vuoto prima di %c è per ignorare eventuali caratteri di newline residui
     * senza di esso viene considerato il carattere x con il newline precedentemte inserito
    */
    printf( "\nInserisci il carattere x da ricercare: " );
    scanf( " %c", &x );

    printf( "\nEcco i tuoi valori:" );
    printf( "\nIntero n: %d", n );
    printf( "\nfile name: %s", fileName );
    printf( "\nCarattere x: %c\n", x );

    unsigned long int numeroTotaleBytes;
    int numeroBytesSingoloThread;
    FILE* file;

    // Apro il file e controllo se l'operazione
    // è andata a buon fine
    file = fopen( fileName, "r" );
    if ( file == NULL ) {
        perror( "Errore nell'apertura del file" );
        exit( EXIT_FAILURE );
    }

    numeroTotaleBytes = getFileSizeBytes( file );
    numeroBytesSingoloThread = numeroTotaleBytes / n;

    printf( "\nIl file è composto da %ld bytes", numeroTotaleBytes );
    printf( "\nOgni Thread dorvà leggere %d bytes\n", numeroBytesSingoloThread );

    pthread_t th[n];

    /**
     * ATTENZIONE:
     * 
     * È molto importante che la struttura sia allocata una per ogni Thread
     * e che inoltre sia allocata dinamicamente, altrimenti
     * si va a lavorare con lo stesso indirizzo di memoria avendo i Thread
     * memoria condivisa, quindi la stuttura sarebbe soggetta a race condiction
     * di conseguenza si procede come di seguito
    */
    for ( int i = 0; i < n; i++ ) {
        struct parameter* parameters = malloc( sizeof( struct parameter ) );
        if ( parameters == NULL ) {
            perror( "Errore nell'allocazione di memoria" );
            exit( EXIT_FAILURE );
        }

        // Inizializzazione parametri della struttura
        parameters->file = file;
        parameters->bytes = numeroBytesSingoloThread;
        parameters->pFile = i * numeroBytesSingoloThread;
        parameters->tid = i;

        if ( pthread_create( &th[i], NULL, routine, ( void* ) parameters ) != 0 ) {
            perror( "Errore nella creazione dei Thread" );
            exit( EXIT_FAILURE );
        }
    }

    int totCount = 0;
    int count = 0;

    // Attendo la terminazione dei Thread
    for ( int i = 0; i < n; i++ ) {
        if ( pthread_join( th[i], &count ) != 0 ) {
            perror( "Errore nella join dei Thread" );
            exit( EXIT_FAILURE );
        }
        totCount += ( int ) count;
    }

    printf( "\nTutti i Thread sono terminati" );
    printf( "\nIl carattere %c è presente nel file %s: %d volte\n", x, fileName, totCount );
    exit( EXIT_SUCCESS );
}