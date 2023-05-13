/**
 * Scrivere un programma che esegua il comando di shell 
 * “ls | sort| grep <path>” con tre processi distinti
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

// direttive del processore
#define READ 0
#define WRITE 1


int main( int argc, char** argv ) {

    // utilizzo 4 descrittori
    int pfdPipe1[2];
    int pfdPipe2[2];
    int pid;

    // creo le pipe effettuando dei controlli
    if( pipe( pfdPipe1 ) == -1 ) {
        perror( "pipe" );
        exit( EXIT_FAILURE );
    }

    if( pipe( pfdPipe2 ) == -1 ) {
        perror( "pipe" );
        exit( EXIT_FAILURE );
    }

    if ( ( pid = fork() ) == -1 ) {
        perror( "fork" );
        exit( EXIT_FAILURE );
    }

    if ( pid != 0 ) { // padre

        // chiudo i descrittori che non utilizzo
        close( pfdPipe1[ READ ] );
        close( pfdPipe2[ READ ] );
        close( pfdPipe2[ WRITE ] );

        // duplico il file descrittore di scrittura 
        dup2( pfdPipe1[ WRITE ], WRITE );

        // chiudo il descrittore utilizzato
        close( pfdPipe1[ WRITE ] );

        // eseguo il comando shell
        execlp( "ls", "ls", NULL );

    } else { // figlio

        // creo un figlio del processo
        if ( ( pid = fork() ) == -1 ) {
            perror( "fork" );
            exit( EXIT_FAILURE );
        }

        if ( !( pid != 0 ) ) { // padre

            // chiudo i descrittori che non utilizzo
            close( pfdPipe1[ WRITE ] );
            close( pfdPipe2[ READ ] );

            // leggo e scrivo
            dup2( pfdPipe1[ READ ], READ );
            dup2( pfdPipe2[ WRITE ], WRITE );

            // chiudo
            close( pfdPipe1[ READ ] );
            close( pfdPipe2[ WRITE ] );

            // eseguo
            execlp( "sort", "sort", "-r", NULL );

        } else { // figlio

            // chiudo i descrittori che non utilizzo
            close( pfdPipe1[ READ ] );
            close( pfdPipe1[ WRITE ] );
            close( pfdPipe2[ WRITE ] );

            // leggo
            dup2( pfdPipe2[ READ ], READ );

            // chiudo
            close( pfdPipe2[ READ ] );

            // eseguo
            execlp( "grep", "grep", argv[1], NULL );
        }
    }
    
    return 0;
}