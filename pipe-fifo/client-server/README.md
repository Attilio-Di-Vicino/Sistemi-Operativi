# Sistema di Comunicazione Client-Server

Questo è un esempio di un'applicazione client-server in cui i client inviano dati al server tramite una pipe FIFO e ricevono una risposta.

## Compilazione

Per compilare il programma, utilizza un compilatore C compatibile (ad esempio, GCC) con il seguente comando:

```
gcc -o client client.c
gcc -o server server.c
```

Questo genererà un file eseguibile chiamato `client` e `server`.

## Esecuzione

Per avviare l'applicazione, segui i passaggi seguenti:

1. Avvia il server in background:

```
$ ./server &
```

2. Avvia i client desiderati. Puoi avviarne più di uno in contemporanea:

```
$ ./client &
$ ./client &
```

    oppure

```
$ for i in 1 2 3 4 5; do ./client & done
```

3. I client invieranno dati al server e riceveranno una risposta. I risultati verranno visualizzati sullo schermo.

## Descrizione del Codice server

Il codice sorgente `server.c` è costituito da diverse fasi:

1. Creazione e apertura della pipe FIFO del server:

- Viene creata la pipe FIFO del server utilizzando `mkfifo`.
- La pipe FIFO del server viene quindi aperta in modalità di sola lettura.

2. Ricezione dei dati dai client e invio della risposta:

- Il server legge i dati inviati dai client tramite la pipe FIFO del server.
- Successivamente, i dati vengono elaborati dal server. In questo caso, tutti i caratteri vengono convertiti in lettere maiuscole.
- Viene creato il nome univoco della pipe FIFO del client corrispondente utilizzando il PID del client ricevuto.
- La pipe FIFO del client viene aperta in modalità di sola scrittura.
- Infine, i dati elaborati vengono inviati al client tramite la pipe FIFO del client.

3. Chiusura del server:

- Il server continua a leggere i dati dai client fino a quando la lettura restituisce un valore inferiore a 0.
- Una volta terminata la lettura, il server chiude la pipe FIFO del server e la rimuove dalla memoria.

## Descrizione del Codice client

Il codice sorgente `client.c` è costituito da due parti principali:

1. Inizializzazione del client:

- Viene aperta una pipe FIFO per scrivere al server.
- Viene creato un nome univoco per la pipe FIFO del client utilizzando il PID (Process ID) del client.
- Viene creata la pipe FIFO del client utilizzando `mkfifo`.
- I dati del client, inclusa l'identificazione del PID, vengono formattati e inviati al server tramite la pipe FIFO del server.

2. Ricezione della risposta dal server:

- Viene aperta la pipe FIFO del client in modalità di lettura.
- Se la pipe FIFO viene aperta con successo, viene letto il messaggio inviato dal server.
- La pipe FIFO del client viene quindi chiusa e rimossa dalla memoria.

## Esempio di Esecuzione

```
34923 sent Hello from 34923, received: HELLO FROM 34923
34924 sent Hello from 34924, received: HELLO FROM 34924
34925 sent Hello from 34925, received: HELLO FROM 34925
34922 sent Hello from 34922, received: HELLO FROM 34922
34921 sent Hello from 34921, received: HELLO FROM 34921
```

Nell'esempio sopra, sono stati avviati 5 client contemporaneamente e ogni client ha inviato il proprio messaggio al server. I messaggi inviati sono stati restituiti correttamente a ciascun client.

## Note

- Assicurati di eseguire prima il server e poi i client per evitare errori di connessione.
- I client possono avviare le richieste in qualsiasi momento dopo l'avvio del server.
- Nell'esempio fornito, il server attende 10 secondi (con `sleep(10)`) prima di iniziare a elaborare le richieste. Questo è solo a scopo dimostrativo e può essere modificato secondo le esigenze.

Questo è un esempio di base di un server di comunicazione client-server utilizzando pipe FIFO.