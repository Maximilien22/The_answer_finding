CC = gcc 

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3
LDFLAGS = -lm
LDLIBS= `pkg-config --libs sdl`

all:	main


main:	tools.o path.o parsing.o vector.o list.o graphics.o
parsing.o:	parsing.c parsing.h
tools.o:	tools.c tools.h
path.o:		path.c path.h
vector.o: 	vector.c vector.h
list.o: 	list.c list.h
graphics.o:	graphics.c graphics.h


.PHONY: clean

clean:
	${RM} main parsing.o path.o tools.o vector.o list.o
