CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS = -lm

all:	main


main:	tools.o path.o parsing.o vector.o list.o
parsing.o:	parsing.c parsing.h
tools.o:	tools.c tools.h
path.o:		path.c path.h
vector.o: 	vector.c vector.h
list.o: 	list.c list.h


.PHONY: clean

clean:
	${RM} main parsing.o path.o tools.o vector.o list.o
