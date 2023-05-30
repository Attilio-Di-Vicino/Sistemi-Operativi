/**
 * 
*/

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

int main() {

    /**
     * si utilizza la system call mkfifo per la creazione di una pipe fifo
     * essà e paragonata ad un file, è simile ad una pipe-anonima, ma ha un nome
     * Sebbene impostiamo i permessi 0777 questi sono filtrati 
     * dalla maschera dell’utente (umask), così come avviene nella normale creazione di file
     *   • I permessi risultanti saranno 755 se, ad esempio, umask è 022
     * 
     * restituisce un valore intero
    */
    int res = mkfifo( "/tmp/my_fifo", 0777 );

    if ( res == 0 )
        printf( "FIFO creata\n" );

    exit( EXIT_SUCCESS );
}

/**
 * Se l'output da terminale è anadato a buon fine, quindi la fifo è stata creata,
 * oppure gia è stata creata si può
 * 
 * • Proviamo a leggere dalla FIFO (vuota)
 *   $ cat < /tmp/my_fifo
 * 
 * • Ora scriviamo nella FIFO (usando un altro terminale, infatti il primo 
 *   comando si sospende in attesa di dati scritti nella FIFO)
 *   $ echo “Sono my_fifo” > /tmp/my_fifo
 * 
 * Alla fine si può rimuovere la fifo con il comando
 *   rm /tmp/my_fifo
*/