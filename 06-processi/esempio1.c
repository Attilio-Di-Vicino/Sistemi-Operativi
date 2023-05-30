#include "../apue/apue.h"

static void my_exit1( void );
static void my_exit2( void );

int main ( void ) {

    if ( atexit( my_exit2 ) != 0 )
        err_sys( "Non posso registrare my_exit2" );

    if ( atexit( my_exit1 ) != 0 )
        err_sys( "Non posso registrare my_exit1" );

    if ( atexit( my_exit1 ) != 0 )
        err_sys( "Non posso registrare my_exit1" );

    print( "Main ha completato\n" );

    return 0;
}

static void my_exit1( void ) {
    printf( "Primo exit handler\n" );
}

static void my_exit2( void ) {
    printf( "Secondo exit handler\n" );
}