/**
 * L'uso della struttura `pthread_attr_t` consente di modificare gli attributi 
 * di default associati ai thread creati con la funzione `pthread_create()`.
 * Questi attributi consentono di personalizzare il comportamento e le caratteristiche dei thread.
 * 
 * Ecco alcuni esempi di attributi che possono essere impostati utilizzando `pthread_attr_t`:
 * 
 * 1. **Detachable**: L'attributo `PTHREAD_CREATE_DETACHED` può essere utilizzato per indicare 
 *      che il thread creato sarà "dettachato", ovvero si libererà automaticamente delle sue risorse
 *      una volta terminato, senza dover essere esplicitamente "joined" dal thread principale.
 * 
 * 2. **StackSize**: L'attributo `PTHREAD_STACK_SIZE` permette di specificare la dimensione della
 *      pila di memoria associata al thread.
 * 
 * 3. **Scope**: L'attributo `PTHREAD_SCOPE_SYSTEM` o `PTHREAD_SCOPE_PROCESS` definisce il livello
 *      di visibilità del thread, indicando se il thread è visibile solo nel contesto del processo
 *      corrente o se può essere visibile anche ad altri processi.
 * 
 * 4. **Scheduling**: Gli attributi `sched_param` e `sched_policy` consentono di impostare la politica
 *      di scheduling e i parametri di priorità del thread.
 * 
 * Per utilizzare gli attributi con la struttura `pthread_attr_t`, è possibile seguire i seguenti passaggi:
 *
 * 1. Dichiarare una variabile `pthread_attr_t` per memorizzare gli attributi desiderati:
 *      `pthread_attr_t attr;`
 * 
 * 2. Inizializzare gli attributi con il valore predefinito: `pthread_attr_init(&attr);`
 * 
 * 3. Modificare gli attributi desiderati utilizzando le funzioni appropriate, come
 *      `pthread_attr_setdetachstate()`, `pthread_attr_setstacksize()`, ecc.
 * 
 * 4. Creare un nuovo thread utilizzando la funzione `pthread_create()` e passare gli attributi
 *      come argomento: `pthread_create(&thread, &attr, thread_function, arg);`
 * 
 * 5. Dopo aver creato il thread, gli attributi possono essere liberati chiamando
 *      `pthread_attr_destroy(&attr);`.
 * 
 * L'utilizzo degli attributi consente di personalizzare il comportamento dei thread in base
 * alle esigenze specifiche dell'applicazione. Tuttavia, in molti casi, è possibile utilizzare
 * i valori di default degli attributi senza doverli modificare esplicitamente.
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void* thread_function( void* arg ) {

    printf( "Sono il thread. Sto eseguendo il mio lavoro.\n" );
    return NULL;
}

int main() {

    pthread_t thread;
    pthread_attr_t attr;

    // Inizializza gli attributi
    pthread_attr_init( &attr );

    // Imposta il thread come detachato
    pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_DETACHED );

    // Crea il thread con gli attributi specificati
    if ( pthread_create( &thread, &attr, thread_function, NULL ) != 0 ) {
        printf( "Errore durante la creazione del thread.\n" );
        return 1;
    }

    // Attendi che il thread termini
    // In questo caso il thread esegue da solo il distacco
    // quindi un eventuale join sul thread non andrebbe a buon fine
    // NB: la sleep non garantisce l'esecuzione del thread
    sleep(1);

    // Dealloca gli attributi
    pthread_attr_destroy( &attr );

    printf( "Il thread ha terminato l'esecuzione.\n" );
    return 0;
}
