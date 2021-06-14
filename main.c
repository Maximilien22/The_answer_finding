#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <err.h>
#include "parsing.h"
#include "tools.h"
#include "path.h"
#include "vector.h"
#include "list.h"
#include "graphics.h"

char* file = "map.osm";


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

	// -------------affiche le todot du graph ---------------------
	/*char * td = todot(g);
	printf("%s\n",td);*/

	


	printGraph(gInfo, g );
	
	
	
	/*free(dist);
	free(pred);*/
	
	// --------------free--------------

	//free(td);
	freeGraphInfo(gInfo);
	freeGraph(g);
	
	


}