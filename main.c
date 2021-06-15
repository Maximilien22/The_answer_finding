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

char* file = "nearpita.osm";



int main (int argc, char** argv)
{
	
	if (argc != 3)
	{
		err(1,"ERROR argument : need 2 arguments, int START and int END.");
	}

	int start = atoi(argv[1]);
	int end = atoi(argv[2]);
	


	// -------------- CONSTRUCTION DU GRAPH ----------------
	GraphInfo * gInfo = NULL;
	Graph *g = NULL;
	
	build_Graph_GraphInfo(&gInfo, &g, &file );


	
    int* pred = calloc(g->order, sizeof(int));
    double* dist = calloc(g->order,sizeof(double));

    struct List* list = initlist();
    A_start(g, gInfo, start, end, dist, pred);
    //Dijkstra(g, start, gInfo, dist, pred);

    double diiiist = get_min_way(start,end,pred,dist,list);

    afficherListe(list);
    printf("Distance = %f\n", diiiist);
   	


   
	// -------------affiche le todot du graph ---------------------
	//char * td = todot(g);
	//printf("%s\n",td);

	


	//displayMap(gInfo, g );
	
	
	
	/*free(dist);
	free(pred);*/
	
	// --------------free--------------

	//free(td);
	freeGraphInfo(gInfo);
	freeGraph(g);
	
	


}