CC = gcc 

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3
LDLIBS= `pkg-config --libs sdl2` -lSDL2_ttf
LIBS = -lm

all:	main



main: main.c Parsing/tools.o ShortestPath/path.o Parsing/parsing.o Parsing/vector.o ShortestPath/list.o Graphics/graphics.o ShortestPath/heap.o
	gcc  $(CFLAGS) $(CPPFLAGS)  $^ $(LDLIBS) -o $@ $(LIBS)
Parsing/parsing.o:	Parsing/parsing.c Parsing/parsing.h
Parsing/tools.o:	Parsing/tools.c Parsing/tools.h
ShortestPath/path.o:		ShortestPath/path.c ShortestPath/path.h
Parsing/vector.o: 	Parsing/vector.c Parsing/vector.h
ShortestPath/heap.o:		ShortestPath/heap.c ShortestPath/heap.h
ShortestPath/list.o: 	ShortestPath/list.c ShortestPath/list.h
Graphics/graphics.o:	Graphics/graphics.c Graphics/graphics.h


.PHONY: clean

clean:
	${RM} main
	${RM} Parsing/*.o Parsing/*.d
	${RM} Graphics/*.o Graphics/*.d
	${RM} ShortestPath/*.o ShortestPath/*.d