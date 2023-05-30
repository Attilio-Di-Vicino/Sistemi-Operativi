#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <stdlib.h>
#include <unistd.h>

int main () {
    
    int sockfd;
    int len;
    
    struct sockaddr_un address;
    int result;
    char ch = 'A';
    
    // creiamo una socket per il client  
    sockfd = socket( AF_UNIX, SOCK_STREAM, 0 );
    
    // definiamo le generalità della socket del server
    address.sun_family = AF_UNIX;
    
    strcpy( address.sun_path, "server_socket" );
    len = sizeof( address );
    
    // connettiamo la nostra socket con quella del server
    result = connect( sockfd, ( struct sockaddr* ) &address, len );
    
    if ( result == -1 ) {
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