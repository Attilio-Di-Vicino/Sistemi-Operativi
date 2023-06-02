/**
 * ESERCIZIO: 4
 * 
 * Scrivere un programma C e Posix sotto Linux che,
 * utilizzando la libreria Pthread crei due Thread, chiamati produttore e consumatore.
 * La risorsa condivisa, è un buffer circolare di dimensione data
 * ( ad esempio 20 ) il cui stato è:
 * 
 * • numero di elemento contenuti: count
 * • puntatore alla prima posizione libera: writepos
 * • puntatore al primo elemento occupato: readpos
 * 
 * Il produttore, inserisce, 20 numeri interi in maniera sequenziale.
 * Il consumatore li estrae sequenzialmente per stamparli.
 * Il programma dovrà prevedere:
 * 
 * • un meccanismo di accesso controllato alla risorsa buffer da parte dei due Thread
 *  ( mutex per il controllo della mutua esclusione nell'accesso al buffer )
 * 
 * • una sincronizzazione tra il produttore ed il consumatore ( Thread ) in caso di
 * 
 *      -> buffer pieno: definizione di una condition per la sospensione del 
 *         produttore se il buffer è pieno ( notfull )
 * 
 *      -> buffer vuoto: definizione di una condition per la sospensione del
 *         consumatore se il buffer è vuoto ( notempty )
*/