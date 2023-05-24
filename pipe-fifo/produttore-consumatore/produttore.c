/**
 * Esempio: comunicazione tra due processi con FIFO (produttore-consumatore)
 * 
 * Produttore (fifo1.c): crea una FIFO se richiesto, poi vi scrive i dati quanto prima.
*/

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>

#define FIFO_NAME "/tmp/my_fifo"
#define BUFFER_SIZE 100 // PIPE_BUF
#define TEN_MEG ( 1024 * 1000 * 10 )

/**
 * Processo produttore
*/
int main() {

    int pipe_fd;
    int res;
    int open_mode = O_WRONLY;
    int bytes_sent = 0;
    char buffer[ BUFFER_SIZE ];

    // Se la FIFO non esiste la creiamo. Poi viene aperta
    // questa condizione controlla se il file FIFO non esiste o non può essere letto
    // in tal caso la crea
    if ( access( FIFO_NAME, F_OK ) == -1 ) {
        
        // creao la fifo con i permessi
        res = mkfifo( FIFO_NAME, 0777 );
        
        if ( res != 0 ) {
            printf( "Could not create fifo %s\n", FIFO_NAME );
            exit(-1);
        }

        printf( "Process %d opening FIFO O_WRONLY\n", getpid() );

        // apro la fifo 
        pipe_fd = open( FIFO_NAME, open_mode );

        printf( "Process %d result %d\n", getpid(), pipe_fd );

        // se la fifo si è aperta correttamente
        if ( pipe_fd != -1 ) {
            
            while( bytes_sent < TEN_MEG ) {
                
                // supponiamo che buffer sia riempito altrove
                res = write( pipe_fd, buffer, BUFFER_SIZE );
                
                if ( res == -1 ) {
                    printf( "Write error on pipe\n" );
                    exit( EXIT_FAILURE );
                }
                
                bytes_sent += res;
            }
            
            close( pipe_fd );
        } else {
            exit( EXIT_FAILURE );
        }
        
        printf( "Process %d finished\n", getpid() );
        exit( EXIT_FAILURE );
    }
}