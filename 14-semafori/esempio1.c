/**
 * ESEMPIO:
 * 
 * Semaforo POSIX con nome
*/

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define FILE_MODE S_IRUSR | S_IWUSR

int main( int argc, char** argv ) {

    int c, flags;
    sem_t* sem;
    unsigned int value;
    flags = O_CREAT;
    value = 1;
    int optind = argc - 1;
    char* optarg;
    
    while ( ( c = getopt( argc, argv, "ei:" ) ) != -1 ) {

        switch (c) {
        
            case 'e': flags |= O_EXCL; 
                break;
        
            case 'i': value = atoi( optarg );
                break;;
        }
    }

    if ( optind != argc-1 ) {
        fprintf( stderr, "usage: %s [-e] [-i initialvalue] <name>", argv[0] );
        exit( EXIT_FAILURE );
    }

    // Apertura semaforo con nome
    sem = sem_open( argv[ optind ], flags, FILE_MODE, value );

    // Chiusura semaforo
    sem_close( sem );

    // Unlink semaforo
    sem_unlink( argv[ optind ] );

    exit( EXIT_SUCCESS );
}