#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

// direttive del processore
#define READ 0
#define WRITE 1

int main() {

    // ho bisogno di 8 descrittori
    int pfdPadrePari[2];
    int pfdPadreDispari[2];
    int pfdPariPadre[2];
    int pfdDispariPadre[2];
    int pfdfigloAB[2];
    int pidA = 10;
    int pidB = 10;
    int numero = 0;
    int risultato = 0;

    // creo le pipe anonime senza effettuare controlli
    pipe( pfdPadrePari );
    pipe( pfdPadreDispari );
    pipe( pfdPariPadre );

    // eseguo un controlli solo come esempio
    if ( pipe( pfdDispariPadre ) == -1 ) {
        perror( "\npipe" );
        exit( EXIT_FAILURE );
    }

    // creo i due figli
    if ( !( pidA = fork() ) ) {
        // solo figlio A entrerà qui dentro
        pidA = 2;
    }

    // solo il padre generara un'altro filglio
    if ( pidA != 2 ) {
        if ( !( pidB = fork() ) ) {
            // solo figlio B entrerà qui dentro
            pidB = 3;
        }
    }

    // filgio pari
    if ( pidA == 2 ) {

        do {

            // chiude tutto apparte lettura da padre e scrittura al padre
            close( pfdPadrePari[ WRITE ] );

            close( pfdPadreDispari[ READ ] );
            close( pfdPadreDispari[ WRITE ] );

            close( pfdPariPadre[ READ ] );

            close( pfdDispariPadre[ READ ] );
            close( pfdDispariPadre[ WRITE ] );

            // leggo da mio padre
            read( pfdPadrePari[ READ ], &numero, sizeof( int ) );
            read( pfdPadrePari[ READ ], &risultato, sizeof( int ) );

            // calcolo il risultato
            risultato += numero;

            // scrivo a mio padre
            write( pfdPariPadre[ WRITE ], &risultato, sizeof( int ) );
        
        } while( numero >= 0 );

        close( pfdPadrePari[ READ ] );
        close( pfdPariPadre[ WRITE ] );
        exit( EXIT_SUCCESS );

    } else if ( pidB == 3 ) { // figlio dispari

        do {

            // chiude tutto apparte lettura da padre e scrittura al padre
            close( pfdPadrePari[ READ ] );
            close( pfdPadrePari[ WRITE ] );

            close( pfdPadreDispari[ WRITE ] );

            close( pfdPariPadre[ READ ] );
            close( pfdPariPadre[ WRITE ] );

            close( pfdDispariPadre[ READ ] );

            // leggo da mio padre
            read( pfdPadreDispari[ READ ], &numero, sizeof( int ) );
            read( pfdPadreDispari[ READ ], &risultato, sizeof( int ) );

            // calcolo il risultato
            risultato += numero;

            // scrivo a mio padre
            write( pfdDispariPadre[ WRITE ], &risultato, sizeof( int ) );

        } while( numero >= 0 );

        close( pfdPadreDispari[ READ ] );
        close( pfdDispariPadre[ WRITE ] );
        exit( EXIT_SUCCESS );

    } else { // padre

        do {

            // prendo il numero in input da tastiera
            printf( "\nInserisci un numero: " );
            scanf( "%d", &numero );

            // il padre scrive al figlio
            close( pfdPadrePari[ READ ] );
            close( pfdPadreDispari[ READ ] );

            // e dopo legge dal figlio
            close( pfdPariPadre[ WRITE ] );
            close( pfdDispariPadre[ WRITE ] ); 

            if ( numero % 2 == 0 ) { // comunico solo con mio figlio pari

                // non chiudo le comunicazioni con mio figlio dispari
                
                // scrivo a mio figlio pari
                write( pfdPadrePari[ WRITE ], &numero, sizeof( int ) );
                write( pfdPadrePari[ WRITE ], &risultato, sizeof( int ) );

                // non chiudo la comunicazione

                // leggo quello che ha scritto mio figlio pari
                read( pfdPariPadre[ READ ], &risultato, sizeof( int ) );
                
            } else { // comunico solo con mio figlio dispari

                // non chiudo le comunicazioni con mio figlio pari
                
                // scrivo a mio figlio dispari
                write( pfdPadreDispari[ WRITE ], &numero, sizeof( int ) );
                write( pfdPadreDispari[ WRITE ], &risultato, sizeof( int ) );

                // non chiudo la comunicazione

                // leggo quello che ha scritto mio figlio pari
                read( pfdDispariPadre[ READ ], &risultato, sizeof( int ) );
            }

            // stampo a video il risultato
            printf( "\nRisultato: %d", risultato );
    
        } while( numero >= 0 );

        // chiudo tutto
        close( pfdPariPadre[ READ ] );
        close( pfdPadrePari[ WRITE ] );
        close( pfdDispariPadre[ READ ] );
        close( pfdPadreDispari[ WRITE ] ); 

        printf( "\nPadre Termino!\n" );
        exit( EXIT_SUCCESS );
    }

    return 0;
}