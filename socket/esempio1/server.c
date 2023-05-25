/**
 * un semplice server locale che accetta le connessioni dal nostro client
 * 
 * • Esso crea la socket del server (socket()),
 *   gli assegna (bind()) un nome,
 *   crea una coda in ascolto (listen()),
 *   e accetta le connessioni (accept())
 * 
 * • Il programma server, in questo esempio, può solo servire un client alla volta
 * 
 * • Legge un carattere dal client, lo incrementa, e lo riscrive
 * 
 * • In sistemi più sofisticati, in cui il server deve eseguire 
 *   più lavoro per conto del client, ciò non sarebbe 
 *   accettabile, poiché altri client non sarebbero in grado 
 *   di connettersi fino a che il server non ha finito
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>

int main() {

    int server_sockfd, client_sockfd;
    int server_len, client_len;
    
    struct sockaddr_un server_address;
    struct sockaddr_un client_address;
    
    /**
     * Rimuoviamo eventuali vecchie socket e creiamo una
     * socket senza nome per il server
    */
    unlink( "server_socket" );
    server_sockfd = socket( AF_UNIX, SOCK_STREAM, 0 );
    
    // Assegnamo un nome alla socket
    server_address.sun_family = AF_UNIX;
    strcpy( server_address.sun_path, "server_socket" );
    
    server_len = sizeof( server_address );
    bind( server_sockfd, ( struct sockaddr* ) &server_address, server_len );
    
    // creiamo una coda di connessione ed attendiamo i client
    listen(server_sockfd, 5);
    
    while( 1 ) {
        
        char ch;
        printf( "server in attesa\n" );
        
        // accetta una connessione
        client_len = sizeof( client_address );
        client_sockfd = accept( server_sockfd, ( struct sockaddr* ) &client_address, &client_len );
        
        // Leggiamo e scriviamo al client su client_sockfd
        read( client_sockfd, &ch, 1 );
        ch++;
        
        write( client_sockfd, &ch, 1 );
        close( client_sockfd );
    }
}