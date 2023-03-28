all: test_graphe

test_graphe: test_graphe.o graphe.o io_graphe.o file.o pile.o
	gcc -o test_graphe test_graphe.o file.o pile.o graphe.o io_graphe.o

graphe.o: graphe.c graphe.h file.h pile.h
	gcc -Wall -c graphe.c

io_graphe.o: io_graphe.c graphe.h
	gcc -Wall -c io_graphe.c


test_graphe.o: test_graphe.c graphe.h 
	gcc -Wall -c test_graphe.c

file.o: file.h file.c
	gcc -Wall -c file.c

pile.o: pile.h pile.c
	gcc -Wall -c pile.c

clean:
	rm -f *.o test_graphe  *~ data/*~
