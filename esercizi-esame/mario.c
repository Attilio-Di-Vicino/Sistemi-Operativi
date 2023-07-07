/**
 * ESAME: Prova Scritta 07/07/2023
 * 
 * Si realizzi un programma in c secondo lo strandard posix sotto linux che presa
 * una directory da riga di comando come parametro, ne stampi i dettagli 
 * ( nome, data di ultimo accesso, permessi ) relativamente solo al file "regolare"
 * di dimensione minore tra tutti quelli presenti in tale directory.
 * In altri termini, se in una determinata direcotry vi sono 4 file regolari,
 * stamperà solo i dettagli( nome, data di ultimo accesso, permessi ) del file di
 * dimensione inferiore contenuto in tale directory.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>

int main( int argc, char *argv[] ) {

    // Controllo dati in input da riga di comando
    if ( argc != 2 ) {
        printf( "\nusage: %s <directory>\n", argv[0] );
        exit( EXIT_FAILURE );
    }

    // Puntatore rappresentante della cartella aperta
    DIR* openDir;

    // Strutture cartella e file contenuti
    struct dirent* directory;
    struct stat fileStat;
    struct stat smallestFileStat;
    char smallestFileName[ 256 ];
    int smallestFileFound = 0;

    // Nome cartella letto in input
    openDir = opendir( argv[1] );

    // Apro la cartella ed assegno il puntatore della cartella
    // al mio rappresentante
    if ( openDir == NULL ) {
        printf( "\nErrore nell'apertura della directory" );
        exit( EXIT_FAILURE );
    }

    // Ciclo l'intero contenuto della cartella
    // assegno il puntatore restituito dalla funzione readdir
    // la quale mi restituirà NULL se non ci sono più file
    while ( ( directory = readdir( openDir ) ) != NULL ) {
        char file_path[ 512 ];
        sprintf( file_path, "%s/%s", argv[1], directory->d_name );

        // Recupero informazioni del file utilizzando stat
        if ( stat( file_path, &fileStat ) == 0 ) {
            // Controllo iniziale per verificare se il file è regolare con S_ISREG
            if ( S_ISREG( fileStat.st_mode ) ) {
                if ( fileStat.st_size < smallestFileStat.st_size ) {
                    smallestFileFound = 1;
                    smallestFileStat = fileStat;
                    strncpy( smallestFileName, directory->d_name, sizeof( smallestFileName ) );
                }
            }
        }
    }

    // Chiusura cartella
    closedir( openDir );

    // Stampa risultato
    if ( smallestFileFound ) {
        printf( "\nFile di dimensione minore:" );
        printf( "\nNome: %s", smallestFileName );
        printf( "\nUltimo accesso: %s", ctime( &smallestFileStat.st_atime ) );
        printf( "Permessi: %o", smallestFileStat.st_mode & ( S_IRWXU | S_IRWXG | S_IRWXO ) );
    } else {
        printf( "\nNessun file regolare trovato nella directory" );
    }

    printf( "\n" );
    exit( EXIT_SUCCESS );
}
