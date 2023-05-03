# Aggiorna un “progetto” sulla base di regole di
# dipendenza contenute in un file con un formato
# speciale detto makefile

# Il makefile contiene un insieme di regole/blocchi

# targetList: dependencyList
# [TAB] commandList 
#	• targetList contiene una lista di file target
#	• dependencyList è una lista di file da cui i file target dipendono
#	• commandList è una lista di zero o più comandi
#	  separati da newline (che “ricostruiscono” i target)
#	• ogni riga di commandList inizia con un TAB!

# makefile esempio
# versione 1
# directory destinazione
prefix = /usr/local
bindir = ${prefix}/bin
all: usaPal usaRev

install:
	cp usaPal usaRev ${bindir}
clean:
	rm *.o usaPal usaRev

usaPal: palindroma.o reverse.o
usaPal.o: palindroma.h
palindroma.o: palindroma.h reverse.h

usaRev: reverse.o
usaRev.o: reverse.h
reverse.o: reverse.h