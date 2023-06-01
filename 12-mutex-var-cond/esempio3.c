/**
 * ESEMPIO: mutex e variabili condizione
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/**
 * Le variabili condizione costituiscono un ulteriore
 * meccanismo di sincronizzazione per i thread
 * 
 * • Mentre i mutex implementano la sincronizzazione
 *   controllando l’accesso dei thread ai dati usando il
 *   polling, le variabili di condizione permettono di
 *   sincronizzare i thread sulla base dell’attuale valore
 *   dei dati (senza polling)
 * 
 * • Una variabile di condizione è sempre associata ad un mutex lock
 * 
 * • Quando un altro thread causerà l’occorrenza di tale evento,
 *   uno o più thread in attesa riceveranno un segnale e si risveglieranno
 * 
 * Le variabili condizione hanno tre componenti: la variabile condizione, un mutex associato, un predicato
 * 
 * • Il predicato è la condizione (o il valore) che un thread controllerà per determinare se deve attendere;
 * 
 * • il mutex è il meccanismo che protegge il predicato;
 * 
 * • la variabile condizione è il meccanismo con cui il thread attende il verificarsi della condizione
*/

struct msg {
    struct msg* m_next;
    /* … altra roba … */
};

struct msg* workq;

// Allocazione statica di varibaile condizione e mutex
pthread_cond_t qready = PTHREAD_COND_INITIALIZER;
pthread_mutex_t qlock = PTHREAD_MUTEX_INITIALIZER;

void process_msg( void ) {

    struct msg* mp;
    for ( ; ; ) {

        pthread_mutex_lock( &qlock );
        
        while( workq == NULL )
            pthread_cond_wait( &qready, &qlock );
        
        mp = workq;
        workq = mp->m_next;

        pthread_mutex_unlock( &qlock );
        // ora elabora il messaggio mp
    }
}

void enqueue_msg( struct msg* mp ) {

    pthread_mutex_lock( &qlock );
    mp->m_next = workq;
    workq = mp;
    pthread_mutex_unlock( &qlock );
    pthread_cond_signal( &qready );
}