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
#define TEN_MEG (1024 * 1000 * 10)

int main() {

    int pipe_fd;
    int res;
    int open_mode = O_WRONLY;
    int bytes_sent = 0;
    char buffer[ BUFFER_SIZE ];

    if (access(FIFO_NAME, F_OK) == -1) {
        
        res = mkfifo(FIFO_NAME, 0777);
        
        if (res != 0) {
            printf("Could not create fifo %s\n", FIFO_NAME);
            exit(-1);
        }

        printf("Process %d opening FIFO O_WRONLY\n", getpid());

        pipe_fd = open(FIFO_NAME, open_mode);

        printf("Process %d result %d\n", getpid(), pipe_fd);

        if (pipe_fd != -1) {
            
            while(bytes_sent < TEN_MEG) {
                
                // supponiamo che buffer sia riempito altrove
                res = write(pipe_fd, buffer, BUFFER_SIZE);
                
                if (res == -1) {
                    printf("Write error on pipe\n");
                    exit(1);
                }
                
                bytes_sent += res;
            }
            
            close(pipe_fd);
        } else {
            exit(1);
        }
        
        printf("Process %d finished\n", getpid());
        exit(1);
    }
}