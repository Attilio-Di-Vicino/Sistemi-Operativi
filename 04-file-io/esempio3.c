/**
 * ESEMPIO: duplicare i descrittori di file
*/

#include <stdio.h>
#include <fcntl.h>

int main ( void ) {

    int fd1, fd2, fd3;

    fd1 = open( "test.txt", O_CREAT | O_RDWR, 0600 );
    printf( "fd1 = %d\n", fd1 );
    write( fd1, "Cosa sta", 8 );

    // Effettua una copia di fd1
    fd2 = dup( fd1 );
    printf( "fd2 = %d\n", fd2 );
    write( fd2, " accadendo", 10 );

    // Chiude lo standard input
    close(0);

    // Effettua un’altra copia di fd
    fd3 = dup( fd1 );
    printf( "fd3 = %d\n", fd3 );
    write( 0, " al contenuto", 13 );

    // Duplica il canale 3 sul canale 2
    dup2( 3, 2 );
    write (2, "?\n", 2);

    return 0;
}