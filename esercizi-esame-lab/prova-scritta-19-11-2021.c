/**
 * ESAME: Prova Scritta 19/11/2021
*/

/**
 * 9 punti ( obbligatorio )
 * 
 * Un processo padre crea M ( con M numero pari ) processi figli. Ciascun processo figlio Fi è identificato
 * da una variabile intera i ( i = 0, 1, 2, 3, ..., M - 1 ). Si gestiscono due casi:
 * 
 *      1. Se il primo argomento dato in input al programma ( ovvero argv[1] ) è uguale al
 *         carattere 'p' ( che sta per "pari" ) ogni processo filgio Fi con i pari ( ovvero M = 6, F0, F2, F4 )
 *         manda un segnale ( SIGUSR1 ) al processo i + 1 ( ovvero a F1, F3, F5 nel caso in cui M = 6 ).
 * 
 *      2. Se invece argv[1] è uguale al carattere 'm' ( che sta per "metà" ) ogni processo figlio Fi
 *         con i < M / 2 ( ovvero se M = 6, F0, F1, F2 ) manda un segnale ( SIGUSR1 ) all'altra metà dei
 *         processi, ovvero al processo i + M / 2 ( ovvero a F3, F4, F5, nel caso in cui M = 6 ).
*/

/**
 * 
*/