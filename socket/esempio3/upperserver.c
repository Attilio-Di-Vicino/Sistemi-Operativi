// upperserver.c: un server per rendere maiuscole linee di testo

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/times.h>

#define SERVER_PORT 1313
#define LINESIZE 40 // da valutare 

void upperlines( int in, int out ) {
    
    char inputline[ LINESIZE ];
    int len, i;
    
    while ( ( len = recv( in, inputline, LINESIZE, 0 ) ) > 0 ) {

        for ( i = 0; i < len; i++ )
            inputline[i] = toupper( inputline[i] );
        send( out, inputline, len, 0 );
    }
}

int main ( int argc, char **argv ) {

    int sock, client_len, fd;
    struct sockaddr_in server, client;
    
    // impostazione dell’end point della comunicazione
    if ( ( sock = socket( AF_INET, SOCK_STREAM, 0) ) == -1 ) {
        perror( "chiamata alla system call socket fallita" );
        exit( EXIT_FAILURE );
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl( INADDR_ANY );

    // leghiamo l'indirizzo all’end point della comunicazione
    if ( bind( sock, ( struct sockadrr* ) &server, sizeof( server ) ) == - 1 ) {
        perror( "chiamata alla system call bind fallita" );
        exit( 2 );
    }

    listen( sock, 1 );
    
    // gestione delle connessioni dei client
    while ( 1 ) {

        client_len = sizeof( client );
        if ( ( fd = accept( sock, ( struct sockaddr* ) &client, &client_len ) ) < 0 ) {
            perror( "accepting connection" );
            exit( 3 );
        }
        fprintf( stdout, "Aperta connessione.\n" );
        send( fd, "Benvenuto all'UpperServer!\n", 27, 0 );
        upperlines( fd, fd );
        close( fd );
        fprintf( stdout, "Chiusa connessione.\n" );
    }
}