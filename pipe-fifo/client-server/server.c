#include "client.h"
#include <ctype.h>

int main() {
    
    int server_fifo_fd, client_fifo_fd;
    struct data_to_pass_st my_data; // struttura da leggere e restituire
    int read_res;
    char client_fifo[256]; // per il nome della FIFO del client
    char *tmp_char_ptr;

    // creazione fifo
    mkfifo( SERVER_FIFO_NAME, 0777 );

    // apertura fifo
    server_fifo_fd = open( SERVER_FIFO_NAME, O_RDONLY );

    if ( server_fifo_fd == -1 ) {
        printf( "Server fifo failure\n" );
        exit( EXIT_FAILURE );
    }

    sleep( 10 ); // accodiamo i client per scopi dimostrativi
    
    do {

        // leggo dalla pipe
        read_res = read(server_fifo_fd, &my_data, sizeof(my_data));

        if ( read_res > 0 ) {

            /* In questa prossima fase, eseguiamo qualche operazione sui dati appena
            letti dal client. Convertiamo tutti i caratteri in lettere maiuscole e
            combiniamo il CLIENT_FIFO_NAME con il pid del client ricevuto. */
            tmp_char_ptr = my_data.some_data;

            while ( *tmp_char_ptr ) {
                *tmp_char_ptr = toupper(*tmp_char_ptr);
                tmp_char_ptr++;
            }

            sprintf(client_fifo,CLIENT_FIFO_NAME,my_data.client_pid);
            
            /* Poi restituiamo i dati elaborati aprendo la pipe del
            client in sola scrittura e modalità bloccante. Infine
            arrestiamo la FIFO del server chiudendo il file e
            facendo l’unlink della FIFO. */
            client_fifo_fd = open(client_fifo,O_WRONLY);
            
            if ( client_fifo_fd != -1 ) {
                write( client_fifo_fd, &my_data, sizeof( my_data ) );
                close( client_fifo_fd );
            }
        }
    } while ( read_res > 0 );

    /* chiude il do */
    close( server_fifo_fd );
    unlink( SERVER_FIFO_NAME );
    exit( EXIT_SUCCESS );
}