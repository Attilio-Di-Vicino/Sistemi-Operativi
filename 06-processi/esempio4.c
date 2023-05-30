/**
 * ESEMPIO: vfork()
*/

#include "../apue/apue.h"

// Variabile esterna (blocco dati inizializzati) 
int glob = 6; 

int main( void ) {
    
    // Variabile automatica sullo stack
    int var; 
    pid_t pid;
    var = 88;
    
    printf( "prima della fork\n" );

    if ( ( pid = vfork() ) < 0 ) {
        err_sys( "errore della vfork" );
    } else if ( pid == 0 ) {
        // figlio
        glob++;
        // modifica le variabili
        var++;
        // il figlio finisce
        _exit(0);
    }

    // il padre continua qui
    printf( "pid = %d, glob = %d, var = %d\n", getpid(), glob, var );
    exit(0);
}