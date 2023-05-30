#include "../apue/apue.h"
#include <sys/types.h>
#include <sys/wait.h>

/**
 * • Scrivere un programma in C e Posix sotto Linux che, preso un argomento intero positivo da riga di comando, 
 *   gestisca la seguente situazione:
 * 
 * • genera due figli A e B e
 * 
 *       • se l'argomento è PARI invia un segnale SIGUSR1 alla ricezione del
 *         quale il figlio A calcola il cubo del numero passato come argomento da linea di comando,
 *         mentre il figlio B stampa un messaggio di arrivederci e termina.
 *       
 *       • se l'argomento è DISPARI invia un segnale SIGUSR2 alla ricezione del
 *         quale il figlio B calcola il reciproco del numero passato comeargomento,
 *         attende per un numero di secondi pari al doppio del
 *         numero passato come argomento ed invia un segnale SIGUSR1 al
 *         processo A dopodiché termina l'esecuzione. Il figlio A, invece, attendela ricezione del
 *         segnale SIGUSR1, stampa un messaggio e termina.
*/

// variabili globali
int pidA, pidB, intero, segnale;

static void gestore( int signo );
int main( void ) {

    int cubo_int;
    float rec_int;

    // assegno ad ogni segnale il gestore
    // senza effettuare controlli
    signal( SIGUSR1, gestore );
    signal( SIGUSR2, gestore );

    printf( "Inserisci un numero intero: " );
    scanf( "%d", &intero );

    // genero figlio A
    pidA = fork();

    if ( pidA == 0 ) {
        pause();
        
        if ( segnale == SIGUSR1 ) {
            cubo_int = intero * intero * intero;
            printf( "Figlio A: il cubo dell'intero è: %d\n", cubo_int );
            exit(0);
        } else {
            printf( "Figlio A: intero DISPARI.. Attendo un segnale da B!\n" );
            pause();

            if ( segnale == SIGUSR1 )
                printf( "Figlio A: segnale arrivato... TERMINO!\n" );
            exit(0);
        }
    }

    // genero figlio B
    pidB = fork();

    if ( pidB == 0 ) {
        pause();
        
        if ( segnale == SIGUSR1 ) {
            printf( "Figlio B: intero PARI.. Arrivederci!\n" );
            exit(0);
        } else {
            rec_int = ( float ) 1 / intero;
            printf( "Figlio B: reciproco intero ricevuto: %f\n", rec_int );
            sleep( 2 * intero );
            kill( pidA, SIGUSR1 );
            exit(0);
        }
    }

    // padre manda il segnale in base
    // al tipo di dato ricevuto in input
    if ( ( pidA && pidB ) != 0 ) {
        sleep( 1 );

        if ( intero % 2 == 0 ) {
            kill( pidA, SIGUSR1 );
            kill( pidB, SIGUSR1 );
        } else {
            kill( pidA, SIGUSR2 );
            kill( pidB, SIGUSR2 );
        }
    }

    wait( NULL );
    wait( NULL );
    exit(0);
}

/**
 * Il gestore è definito come procedura statica
 * ed va a settare la variabile globale segnale
 * non viene fatto manualmente nel main, ma
 * il kernel manda il segnale al processo destinatario
 * 
 * quindi la funzione gestore viene chiamata quando uno dei figli 
 * riceve un segnale 
 * 
 * @param signo segnale inviato SIGUSR1 o SIGUSR2
*/
static void gestore( int signo ) {
    segnale = signo;
}