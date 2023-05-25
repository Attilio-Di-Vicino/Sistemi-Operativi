#include "client.h"
#include <ctype.h>

int main() {

    int server_fifo_fd, client_fifo_fd;
    struct data_to_pass_st my_data;
    char client_fifo[ 256 ];
    
    // apertura in scrittura della pipe fifo
    server_fifo_fd = open( SERVER_FIFO_NAME, O_WRONLY );
    
    if ( server_fifo_fd == -1 ) { 
        fprintf( stderr, "Sorry, no server\n" );
        exit( EXIT_FAILURE );
    }
    
    // salvo io mio pid
    my_data.client_pid = getpid();

    /**
     * sprintf identica a printf tranne per il fatto che l’output viene
     * inserito nell’array puntato da my_data.some_data invece che su stdout
    */
    sprintf( client_fifo, CLIENT_FIFO_NAME, my_data.client_pid );
    
    // creo una pipe fifo
    if ( mkfifo( client_fifo, 0777 ) == -1 ) {
        fprintf( stderr, "Sorry, can't make %s\n", client_fifo );
        exit( EXIT_FAILURE );
    }
    
    /**
     * I dati dei client sono inviati al server. Successivamente è
     * aperta la FIFO client (sola lettura, modalità bloccante) ed i
     * dati sono letti dal client. Infine, la FIFO del server è
     * chiusa e la FIFO client rimossa dalla memoria.
    */
    sprintf( my_data.some_data, "Hello from %d", my_data.client_pid );
    printf( "%d sent %s, ", my_data.client_pid, my_data.some_data );
    
    write( server_fifo_fd, &my_data, sizeof( my_data ) );
    
    client_fifo_fd = open( client_fifo, O_RDONLY );
    
    if ( client_fifo_fd != -1 ) {
        if ( read( client_fifo_fd, &my_data, sizeof( my_data ) ) > 0 )
            printf( "received: %s\n", my_data.some_data );
        close( client_fifo_fd );
    }
    
    close( server_fifo_fd );
    unlink( client_fifo );
    exit( EXIT_SUCCESS );
}

/**
 * • Per testare l’applicazione, eseguiamo una copia
singola del server e più copie dei client
• Per avviarli e chiuderli tutti nello stesso tempo
usiamo i seguenti comandi di shell
$ server &
$ for i in 1 2 3 4 5
do
client &
done
$

• Partono 1 processo server e 5 processi client
34923 sent Hello from 34923, received: HELLO FROM 34923
34924 sent Hello from 34924, received: HELLO FROM 34924
34925 sent Hello from 34925, received: HELLO FROM 34925
34922 sent Hello from 34922, received: HELLO FROM 34922
34921 sent Hello from 34921, received: HELLO FROM 34921
• Le diverse richieste dei client sono intrecciate
(interleaved) ed ogni client riceve gli opportuni
dati ad esso restituiti
*/