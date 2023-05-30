#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>

int main() {

    int server_sockfd, client_sockfd;
    int server_len, client_len;
    
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    
    // crea un socket senza nome per il server
    server_sockfd = socket( AF_INET, SOCK_STREAM, 0 );
    
    // assegna un nome alla socket
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr( "127.0.0.1" );
    server_address.sin_port = 9734;
    server_len = sizeof( server_address );
    
    bind( server_sockfd, ( struct sockaddr* ) &server_address, server_len );
    
    // creiamo una coda di connessione ed attendiamo i client
    listen( server_sockfd, 5 );
    
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