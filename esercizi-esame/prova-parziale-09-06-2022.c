/**
 * PARZIALE: Prova Scritta 14/07/2022
*/

/**
 * 6 punti
 *
 * Scrivere un programma in C che, in ambiente Linux, presa una directory come parametro da riga di comando,
 * provvederà a scorrerla allo scopo di stampare i nomi di tutti i file regolari e le loro dimensioni contenuti in tale
 * directory se questi hanno i diritti di scrittura e lettura per il proprietario e per gli altri.
*/

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>

int main( int argc, char* argv[] ) {

    // Controllo dati in input da riga di comando
    if ( argc != 2 ) {
        perror( "Errore nei dati in input" );
        exit( EXIT_FAILURE );
    }

    // Puntatore rappresentante della cartella aperta
    DIR* openDir;

    // Strutture cartella e file contenuti
    struct dirent* directory;
    struct stat fileC;

    // Nome cartella letto in input
    char* pathDir = argv[1];

    // Apro la cartella ed assegno il puntatore della cartella
    // al mio rappresentante
    if ( ( openDir = opendir( pathDir ) ) == NULL ) {
        perror( "Errore nell'apertura della cartella" );
        exit( EXIT_FAILURE );
    }

    // Stampo il nome della cartella
    printf( "\nCartella: %s aperta con successo.\n", pathDir );

    // Ciclo l'intero contenuto della cartella
    // assegno il puntatore restituito dalla funzione readdir
    // la quale mi restituirà NULL se non ci sono più file
    while ( ( directory = readdir( openDir ) ) != NULL ) {

        // Nome del file corrente
        char* fileName = directory->d_name;
        
        // Informazioni utili
        printf( "\nVerificando: <path-directory>/%s -> ", fileName );

        // Recupero informazioni del file utilizzando stat
        // if ( stat( dir->d_name, &fileC ) == -1 ) {
        //     perror( "Errore nel recupero delle informazioni" );
        //     exit( EXIT_FAILURE );
        // } 
        stat( fileName, &fileC );

        // Controllo iniziale per verificare se il file è regolare con S_ISREG
        // Controlli suucessivi per verificare se l'utente
        // ha i diritti di lettura e scrittura sul file
        if ( S_ISREG( fileC.st_mode ) &&
            ( fileC.st_mode & S_IRUSR ) && ( fileC.st_mode & S_IWUSR ) &&
            ( fileC.st_mode & S_IRGRP ) && ( fileC.st_mode & S_IWGRP ) &&
            ( fileC.st_mode & S_IROTH ) && ( fileC.st_mode & S_IWOTH ) ) {

            // Se supera i precendenti controlli   
            printf( "bytes: %ld", fileC.st_size );

        } else {

            // Se non supera i precendenti controlli 
            printf( "non rispetta le verifiche." );
        }
    }

    printf( "\n" );
    exit( EXIT_SUCCESS );
}