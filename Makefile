assambler: First_round.o Second_round.o General.o Main.o 
	gcc -ansi -pedantic -Wall First_round.o Second_round.o General.o Main.o -o assambler
First_round.o: First_round.c Header.h
	gcc -c -ansi -pedantic -Wall First_round.c -o First_round.o
Second_round.o: Second_round.c Header.h
	gcc -c -ansi -pedantic -Wall Second_round.c -o Second_round.o
General.o: General.c Header.h
	gcc -c -ansi -pedantic -Wall General.c -o General.o
Main.o: Main.c Header.h
	gcc -c -ansi -pedantic -Wall Main.c -o Main.o

