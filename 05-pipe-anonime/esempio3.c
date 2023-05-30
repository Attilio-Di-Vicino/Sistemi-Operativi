/**
 * • Quando un processo “scrittore” invia più messaggi di lunghezza variabile tramite una pipe, 
 *   occorre fissare un protocollo di comunicazione che permetta al processo “lettore”
 *   di individuare la fine di ogni singolo messaggio
 *      
 * • Alcune possibilità sono:
 *      
 * • inviare la lunghezza del messaggio (dato di dimensione fissa enota) prima del messaggio stesso
 *      
 * • terminare un messaggio con un carattere speciale come '\0' o un newline
 *      
 * • Più in generale, il protocollo stabilisce la sequenza di messaggi attesa delle due parti
 * 
 * Semplice protocollo di comunicazione tramite pipe anonima.
 * Dovendo inviare messaggi di lunghezza variabile ogni messaggio è preceduto
 * da un intero che fornisce la sua lunghezza in caratteri
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define READ 0 // Estremità in lettura della pipe
#define WRITE 1 // Estremità in scrittura della pipe

char *msg[3] = { "Primo 14 byte", "Secondo 16 byte", "Terzo 14 byte" };

int main ( void ) {

    int fd[2], i, length, bytesRead;
    char buffer[100]; // Buffer del messaggio
    pipe( fd ); // Crea una pipe anonima

    if ( fork () == 0 ) { // Figlio, scrittore
        close( fd[ READ ] ); // Chiude l’estremità inutilizzata

        for ( i = 0; i < 3; i++ ) {
            length = strlen ( msg[i] ) + 1 ; // include \0
            write( fd[ WRITE ], &length, sizeof( int ) );
            write( fd[ WRITE ], msg[i], length ); 
        }
        close( fd[ WRITE ]); // Chiude l’estremità usata
    } else { // Genitore, lettore

        close( fd[ WRITE ] ); // Chiude l’estremità non usata

        while ( read( fd[ READ ], &length, sizeof( int ) ) ) {
            bytesRead = read ( fd[ READ ], buffer, length );
            if ( bytesRead != length ) {
                printf( "Errore!\n" );
                exit(1);
            }
            printf( "Padre -> Letti %d byte: %s\n", bytesRead, buffer );
        }
        close( fd[ READ ] ); // Chiude l’estremità usata
    }
    exit(0);
}