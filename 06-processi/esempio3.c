/**
 * ESEMPIO: le modifiche alle variabili del 
 * processo figlio non si estendono ai valori delle 
 * variabili del processo padre
*/

#include "../apue/apue.h"

// variabile esterna (blocco dati inizializzati)
int glob = 6;
char buf[] = "una write sullo stdout\n";

int main( void ) {

    // variabile automatica sullo stack
    int var;
    pid_t pid;
    
    var = 88;
    
    if ( write( STDOUT_FILENO, buf, sizeof( buf ) - 1 ) != sizeof( buf ) - 1 )
        err_sys( "errore della write" );
    
    printf( "prima della fork\n" );
    
    if ( ( pid = fork() ) < 0 ) {
        err_sys( "errore della fork" );
    } else if ( pid == 0 ) {
        // figlio
        glob++;
        // modifica le variabili
        var++;
    } else {
        sleep(2);
        /* padre */
    }
    printf( "pid = %d, glob = %d, var = %d\n", getpid(), glob, var );
    exit(0);
}