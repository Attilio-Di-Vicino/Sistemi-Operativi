/**
 * Esempio 1
 * Padre Scrive - Figlio Legge
 * 
 * La sincronizzazione tra il processo padre e il processo figlio nella comunicazione tramite pipe avviene in modo implicito.
 * Quando un processo scrive su una pipe, i dati vengono messi in una coda interna alla pipe.
 * Se il processo figlio tenta di leggere dalla pipe ma non ci sono ancora dati disponibili,
 * la funzione read blocca il processo fino a quando non ci sono dati da leggere.
 * 
 * Allo stesso modo, se il processo padre tenta di scrivere sulla pipe ma non c'è spazio sufficiente nella coda interna,
 * la funzione write blocca il processo fino a quando c'è spazio disponibile per scrivere.
 * 
 * In questo modo, il processo figlio non leggerà mai dati non validi o incompleti,
 * poiché la funzione read aspetterà finché non ci sono dati disponibili per la lettura.
 * Allo stesso modo, il processo padre non sovrascriverà mai i dati non letti, 
 * poiché la funzione write aspetterà finché non c'è spazio sufficiente nella coda interna per scrivere.
 * 
 * In generale, l'uso di pipe in C garantisce una sincronizzazione tra i processi che utilizzano la pipe,
 * consentendo una comunicazione affidabile e sicura tra processi concorrenti.
 */

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define SIZE 1024
#define READ 0
#define WRITE 1

int main( int argc, char* argv[] ) {

    /**
     * pfd è un array di 2 elementi il quale
     * rappresenta i descrittori in lettura e scrittura
    */
    int pfd[2] ;
    int nread;
    int pid;
    char buf[ SIZE ];

    /**
     * In questo caso viene creata una pipe
     * se la funzione ritorna -1 allora la creazione è fallita
    */
    if ( pipe( pfd ) == - 1 ) {
        perror( "pipe() fallita)" );
        exit( -1 );
    }

    /**
     * Viene esgeuita una fork per la creazione
     * del secondo processo comunicante con esso 
     * ( processo padre - processo figlio )
    */
    if( ( pid = fork() ) < 0 ) {
        perror( "fork() fallita" );
        exit( -2 );
    }

    // se sono il figlio
    if( pid == 0 ) {

        // chiudo il descrittore di scrittura, perchè io leggo solo
        close( pfd[ WRITE ] );
        // leggo dal buffer
        while ( ( nread = read( pfd[ READ ], buf, SIZE ) ) != 0 )
            printf( "il figlio legge: %s\n", buf );
        // chiudo il descrittore in lettura avendo letto
        close( pfd[ READ ] );
    
    // se sono il padre
    } else { 

        // chiudo il descrittore di lettura, dovendo solo scrivere
        close( pfd[ READ ] );
        // scrivo nel buffer
        strcpy( buf, "Sono tuo padre!" );
        // scrivo nel buffer
        write( pfd[ WRITE ], buf, strlen( buf ) + 1 );
        // chiudo il descrittore in lettura avendo finito di scrivere
        close( pfd[ WRITE ] );

    }
    exit(0);
}