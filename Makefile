CC = gcc
CFLAGS = -Wall -Wextra

all:	parsing	


parsing:	tools.o Dijkstra.o parsing.o list.o
parsing.o:	parsing.c parsing.h
tools.o:	tools.c tools.h
Dijkstra.o: Dijkstra.c Dijkstra.h
list.o: 	list.c list.h


.PHONY: clean

clean:
	${RM} parsing parsing.o
	${RM} tools.o
	${RM} path.o 
	${RM} Dijkstra.o list.o
