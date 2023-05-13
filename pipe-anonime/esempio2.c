#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define READ 0
#define WRITE 1

char *frase;

int main(){

    int fd[ 2 ], bytesRead;
    char message[ 100 ];
    frase = "Questo è il messaggio di 36 byte..";

    pipe( fd );

    if( fork() == 0 ) {

        close( fd[ READ ] );
        write( fd[ WRITE ], frase, strlen( frase ) + 1 );
        close( fd[ WRITE ]);
    } else {

        close( fd[ WRITE ] );
        bytesRead = read( fd[ READ ], message, 100 );
        printf( "Letti %d byte: %s\n", bytesRead, message );
        close( fd[ READ ] );
    }
}