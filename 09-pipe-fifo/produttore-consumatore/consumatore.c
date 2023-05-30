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

/**
 * Proecsso consumatore aperto in lettura
*/
int main() {

    int pipe_fd;
    int res;
    int open_mode = O_RDONLY;
    char buffer[ BUFFER_SIZE ];
    int bytes_read = 0;
    
    printf( "Process %d opening FIFO O_RDONLY\n", getpid() );
    
    // apertura pipe creata precedentemente
    // e ritorna il descrittore della pipe con nome
    // se ritorna -1 c'è un errore
    pipe_fd = open( FIFO_NAME, open_mode );
    
    printf( "Process %d result %d\n", getpid(), pipe_fd );
    
    // se non ci sono stati errori nell'apertura della pipe
    if ( pipe_fd != -1 ) {
        
        // leggi dalla pipe
        do {
            res = read( pipe_fd, buffer, BUFFER_SIZE );
            bytes_read += res;
        } while ( res > 0 );

        // chiusura pipe
        close( pipe_fd );
    } else {
        exit(-1);
    }
    
    printf( "Process %d finished,%d bytes read\n", getpid(), bytes_read );
    exit( EXIT_SUCCESS );
}