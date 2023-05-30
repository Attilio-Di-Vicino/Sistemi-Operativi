/**
 * ESERCIZIO:
 * 
 * Scrivere un programma che accetta un intero n da 
 * riga di comando, crea n thread e poi aspetta la loro terminazione
 * 
 * Ciascun thread aspetta un numero di secondi casuale
 * tra 1 e 10, poi incrementa una variabile globale intera 
 * ed infine ne stampa il valore
 * 
 * NB: a questo punto da programma non si è ancora trattato come argomento
 *     mutex, o semafori, di conseguenza non viene gestita la sincronizzazione
*/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int glob = 0;

/**
 * ATTENZIONE: 
 * 
 * glob è una variabile globale la quale viene utilizzata dagli N thread 
 * quindi di conseguenza è soggetta a race condiction
 * in questo esercizio di esempio non essendo richiesto
 * non vengono implementate sezioni critiche per garantire accesso in mutua esclusione
 * della risorsa condivisa
*/
void* routine( void* argv ) {
    int secondiAttesi = rand() % 10 + 1;
    sleep( secondiAttesi );
    printf( "\nDopo aver atteso %d secondi incremento glob: %d", secondiAttesi, ++glob );
} 

int main( int argc, char* argv[] ) {

    /**
     * Se argc è diverso da 2 c'è un errore nell'invio dei dati
     * questo perche da traccia andiamo a leggere un numero intero n
     * quindi avremo il file eseguibile ed n, quindi 2 dati
    */
    if ( argc != 2 ) {
        perror( "Errore nell'invio dei dati" );
        exit( EXIT_FAILURE );
    }

    // atoi converte una stringa ad un intero
    const int N = atoi( argv[1] );
    printf( "File eseguibile: %s\n", argv[0] );
    printf( "Numero letto in input: %d\n", N );

    pthread_t th[N];

    // Seme generato in base all'orario locale
    srand( time( NULL ) );

    // Creazione N Thread
    for ( int i = 0; i < N; i++ ) {
        if ( pthread_create( &th[i], NULL, routine, NULL ) != 0 ) {
            printf( "Errore nella creazione del thread %d", i + 1 );
            exit( EXIT_FAILURE );
        }
    }

    // Attendo che gli N Thread finiscano la loro esecuzione
    for ( int i = 0; i < N; i++ ) {
        if ( pthread_join( th[i], NULL ) != 0 ) {
            printf( "Errore nella join del thread %d", i + 1 );
            exit( EXIT_FAILURE );
        }
    }

    printf( "\n\nTutti i Thread sono terminati." );
    printf( "\nValore glob finale: %d\n", glob );
    exit( EXIT_SUCCESS );
}