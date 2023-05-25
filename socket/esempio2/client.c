#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>

int main() {
    
    int sockfd;
    int len;
    
    struct sockaddr_in address;
    
    int result;
    char ch ='A';
    
    // creiamo una socket per il client
    sockfd = socket( AF_INET, SOCK_STREAM, 0 );
    
    // assegnamo un nome alla socket
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr( "127.0.0.1" );
    address.sin_port = 9734;
    len = sizeof( address );
    
    // connettiamo la nostra socket con quella del server
    result = connect( sockfd, ( struct sockaddr* ) &address, len );
    
    if ( result ==-1 ) {
        perror( "ops:client 1" );
        exit( EXIT_FAILURE );
    }
    
    // possiamo leggere e scrivere via sockfd
    write( sockfd, &ch, 1 );
    read( sockfd, &ch, 1 );
    
    printf( "char dal server = %c\n", ch );
    
    close( sockfd );
    exit( EXIT_SUCCESS );
}