/**
 * ESEMPIO: thread con passaggio di parametri
*/

#include <pthread.h>
#include <stdio.h>

// parametri per char_print
struct char_print_parms {
    char character;
    int count;
};

void* char_print ( void* parameters ) {

    // cast del puntatore al tipo corretto
    struct char_print_parms* p = ( struct char_print_parms* ) parameters;
    int i;
    
    for ( i = 0; i < p->count; i++ )
        fputc( p->character, stderr );
    
    return NULL;
}

int main() {

    pthread_t tid1;
    pthread_t tid2;

    struct char_print_parms tid1_args;
    struct char_print_parms tid2_args;

    // crea un thread per stampare 30000 ‘x’
    tid1_args.character = 'x';
    tid1_args.count = 30000;

    pthread_create( &tid1, NULL, char_print, ( void* ) &tid1_args );

    // crea un thread per stampare 20000 ‘y’
    tid2_args.character = 'y';
    tid2_args.count = 20000;

    pthread_create( &tid2, NULL, char_print, ( void* ) &tid2_args );
    sleep(1);
    
    return 0;
}