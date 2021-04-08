CC = gcc
CFLAGS = -Wall -Wextra

all:	parsing

parsing:	tools.o parsing.o 
parsing.o:	parsing.c parsing.h
tools.o:	tools.c tools.h

.PHONY: clean

clean:
	${RM} parsing parsing.o
	${RM} tools.o
