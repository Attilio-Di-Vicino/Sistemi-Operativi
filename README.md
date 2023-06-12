# Sistemi-Operativi

Questo repository è dedicato al corso di Sistemi Operativi - Parte Laboratorio nell'ambito dell'esame di Informatica.

## Contenuti

Il repository include i seguenti argomenti principali:

### 1. Makefile

- Concetti fondamentali dei Makefile e il loro utilizzo per automatizzare la compilazione e l'esecuzione dei programmi. gestione delle dipendenze dei file sorgente.

### 2. File I/O

- Operazioni di input/output sui file in ambiente Unix.
- Apertura, lettura, scrittura e chiusura dei file utilizzando le chiamate di sistema appropriate.
- Gestione degli errori durante le operazioni di I/O.

### 3. File e Directory

- Concetti fondamentali dei file e delle directory in Unix.
- Manipolazione dei file e delle directory utilizzando le chiamate di sistema appropriate.
- Creazione, eliminazione, spostamento e rinomina di file e directory.

### 4. Processi

- Concetti di base sui processi in ambiente Unix.
- Creazione, terminazione e gestione dei processi.
- Comunicazione e sincronizzazione tra processi utilizzando pipe anonime e FIFO.
- Utilizzo dei segnali per la gestione degli eventi e delle interruzioni.

### 5. Segnali

- Uso dei segnali per la comunicazione tra processi e per gestire eventi e interruzioni nel sistema operativo.
- Catturare e gestire segnali specifici per implementare comportamenti personalizzati nei programmi.

### 6. Pipe Anonime

- Utilizzo delle pipe anonime per la comunicazione tra processi in ambiente Unix.
- Creare, utilizzare e gestire le pipe anonime per lo scambio di dati tra processi.

### 7. Pipe FIFO

- Concetti e utilizzo delle pipe FIFO per la comunicazione tra processi in ambiente Unix.
- Creare, utilizzare e gestire le pipe FIFO per la trasmissione di dati tra processi.

### 8. IPC Socket

- Introduzione alla comunicazione tra processi utilizzando socket IPC (Inter-Process Communication).
- Creazione, connessione, invio e ricezione di dati attraverso socket.
- Comunicazione tra processi locali o remoti tramite socket.

### 9. Thread

- Concetti di base sui thread e sulla programmazione multithread.
- Creazione, terminazione e gestione dei thread.
- Sincronizzazione dei thread utilizzando mutex, variabili di condizione e semafori.

### 10. Mutex e Variabili di Condizione

- Utilizzo dei mutex e delle variabili di condizione per sincronizzare l'accesso concorrente alle risorse condivise tra i thread.
- Vengono utilizzate queste strutture per prevenire condizioni di race e garantire la corretta sincronizzazione tra i thread.

### 11. Semafori

- Concetti e utilizzo dei semafori per gestire l'accesso concorrente alle risorse tra i thread.
- Vengono utilizzati i semafori per implementare meccanismi di sincronizzazione e risolvere problemi di mutua esclusione e cooperazione tra i thread.

### 12. Esercizi Utili

- esercizi pratici degli argomenti trattati nel corso di Sistemi Operativi.

## Utilizzo

Per utilizzare il repository, segui i seguenti passaggi:

1. Clona il repository sul tuo sistema locale:

```
git clone https://github.com/Attilio-Di-Vicino/Sistemi-Operativi.git
```

2. Esplora le diverse cartelle e i file relativi agli argomenti che desideri approfondire.

## Contributi

Se desideri contribuire a questo repository, segui queste linee guida:

1. Forka il repository e clona la tua copia sul tuo sistema locale.

2. Crea un nuovo branch per le tue modifiche:

```
git checkout -b nome_del_tuo_branch
```

3. Apporta le modifiche desiderate, aggiungi nuovi contenuti o correggi eventuali errori.

4. Effettua il commit delle tue modifiche:

```
git commit -m "Descrizione delle modifiche"
```

5. Fai push delle modifiche nel tuo repository:

```
git push origin nome_del_tuo_branch
```

6. Invia una richiesta pull con le tue modifiche per revisione.

## Note

- Assicurati di aver installato un ambiente Unix compatibile e le librerie necessarie per eseguire correttamente gli esempi di codice forniti nel repository.
- Consulta i file README all'interno di ciascuna cartella specifica per ulteriori istruzioni e requisiti specifici relativi agli argomenti trattati.