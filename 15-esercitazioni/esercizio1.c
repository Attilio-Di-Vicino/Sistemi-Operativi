/**
 * ESERCIZIO: 1
 * 
 * Si realizzi un programma in C e Posix sotto Linux che, con l’ausilio della libreria Pthread
 * 
 * • lancia n thread per calcolare la somma degli elementi di ciascuna 
 *   riga di una matrice nxn di interi generati casualmente in un 
 *   intervallo [0,255], allocata dinamicamente
 * 
 * • Il calcolo della somma degli elementi di ciascuna riga deve essere
 *   effettuato concorrentemente su tutte le righe, secondo la seguente modalità:
 *      
 *      -> il thread i-esimo con i pari, calcola la somma degli 
 *         elementi di indice pari della riga i-esima
 * 
 *      -> il thread con indice i dispari, calcola la somma degli elementi di
 *         indice dispari della riga i-esima.
 * 
 * • Calcolate le somme parziali, si provvederà a ricercarne il minimo ed a stamparlo a video.
*/