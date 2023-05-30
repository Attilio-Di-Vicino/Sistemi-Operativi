/**
 * • Completare l'esempio del maiuscolatore, scrivendo il
 *   codice del client (caso del modello connection oriented).
*/

// La struttura di quest'ultimo sarà la seguente:

#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>

void main() {
    
    int sockfd;
    
    if( ( sockfd = socket( AF_INET, SOCK_STREAM, 0  )) == -1 ) {
        perror( "chiamata alla system call socket fallita" );
        exit( EXIT_FAILURE );
    }
    
    // connessione al server
    // invio e ricezione della stringa
    // chiusura della connessione
}