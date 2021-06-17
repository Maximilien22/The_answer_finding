#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <err.h>
#include "Parsing/parsing.h"
#include "Parsing/tools.h"
#include "Parsing/vector.h"

#include "ShortestPath/path.h"
#include "ShortestPath/list.h"
#include "Graphics/graphics.h"

char* file = "Maps/map.osm";



int main ()
{
	
	/*if (argc != 3)
	{
		err(1,"ERROR argument : need 2 arguments, int START and int END.");
	}

	int start = atoi(argv[1]);
	int end = atoi(argv[2]);*/
	


	// -------------- CONSTRUCTION DU GRAPH ----------------
	GraphInfo * gInfo = NULL;
	Graph *g = NULL;
	
	build_Graph_GraphInfo(&gInfo, &g, &file );
	
	freeHashTable(g);

	// -------------affiche le todot du graph ---------------------
	/*char * td = todot(g);


	

*/
	displayMap(gInfo, g );
	
	// --------------free--------------

	freeGraphInfo(gInfo);
	freeGraph(g);
	
	


}