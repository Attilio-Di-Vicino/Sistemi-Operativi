#include <signal.h>
#include <stdio.h>
#include <signal.h>

// compila il file gcc
// esegui il file a.out
// manda segnale ctrl C -> ^C

int segnale;
static void gestore( int signo );
 
int main() {
	if ( signal( SIGINT, gestore ) == SIG_ERR )	
		printf( "\nNon sono riuscito a catturare il segnale!" );

	do {
		pause();

		if ( segnale == SIGINT )
			printf("\n Segnale ricevuto! ");


	} while( segnale == SIGINT );
}

static void gestore( int signo ) {
	signal( SIGINT, gestore );
	segnale = signo;
}