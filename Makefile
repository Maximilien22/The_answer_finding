CC = gcc 

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3
LDLIBS= `pkg-config --libs sdl` -lSDL2 -lSDL2_image
LIBS = -lm

all:	main



main: main.c tools.o path.o parsing.o vector.o list.o graphics.o heap.o
	gcc  $(CFLAGS) $(CPPFLAGS)  $^ $(LDLIBS) -o $@ $(LIBS)
parsing.o:	parsing.c parsing.h
tools.o:	tools.c tools.h
path.o:		path.c path.h
vector.o: 	vector.c vector.h
list.o: 	list.c list.h
graphics.o:	graphics.c graphics.h
heap.o:		heap.c heap.h


.PHONY: clean

clean:
	${RM} main *.d *.o
