#include "../Apue/apue.h"

// compila il file gcc
// esegui il file a.out
// apri un nuovo terminale
// esegui il comando ps -xa 
// prendi il codice dell'eseguibile
// manda kill -INT codice

/**
 * 1. Scrivere un programma che intercetta CTRL-c ed invece di terminare scrive su stdout “non voglio terminare”
 * 
 * 2. Scrivere un programma C che riceva in input da tastiera duenumeri interi, a e b, e ne stampi a video:
 *   • la somma "a+b" solo quando riceve il segnale SIGUSR2
 *   • la differenza "a-b" quando riceve il segnale SIGUSR1
 *   • Il programma esce quando riceve SIGINT
*/

int segnale;
static void gestore( int signo );
 
int main() {
	int a = 20, b = 10;

    // aggancio i rispettivi segnali effettuando
    // un controllo del successo di cattura del segnale
	if ( signal( SIGINT, gestore ) == SIG_ERR )	
		printf( "\nNon sono riuscito a catturare il segnale!" );
	else if ( signal( SIGUSR1, gestore ) == SIG_ERR )	
		printf( "\nNon sono riuscito a catturare il segnale!" );
	else if ( signal( SIGUSR2, gestore ) == SIG_ERR )	
		printf( "\nNon sono riuscito a catturare il segnale!" );

	do {
		pause();
		if ( segnale == SIGINT )
			printf("\n Non voglio terminare! ");
		else if ( segnale == SIGUSR1 )
			printf( "\n Somma: %d", a + b );
		else if ( segnale == SIGUSR2 )
			printf( "\n Sottrazione: %d", a - b );
	} while( segnale == SIGINT || segnale == SIGUSR1 || segnale == SIGUSR2 );
}

/**
 * Il gestore è definito come procedura statica
 * ed va a settare la variabile globale segnale
 * non viene fatto manualmente nel main, ma
 * il kernel manda il segnale al processo destinatario
 * inoltre riaggancia nuovamente tutti i segnali al gestore
 * in modo tale da poter ripetere la procedura più volte
 * 
 * quindi la funzione gestore viene chiamata quando 
 * il processo in esecuzione riceve un segnale 
 * 
 * @param signo segnale inviato
*/
static void gestore( int signo ) {
	signal( SIGUSR1, gestore );
	signal( SIGUSR2, gestore );
	signal( SIGINT, gestore );
	segnale = signo;
}