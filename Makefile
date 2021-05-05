CC = gcc
CFLAGS = -Wall -Wextra

all:	parsing	


parsing:	tools.o parsing.o path.o
parsing.o:	parsing.c parsing.h
tools.o:	tools.c tools.h
path.o:		path.c 	path.h

.PHONY: clean

clean:
	${RM} parsing parsing.o
	${RM} tools.o
	${RM} path.o 
