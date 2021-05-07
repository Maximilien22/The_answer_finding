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


int main (int argc, char** argv)
{
	if (argc != 3)
	{
		err(1,"ERROR argument : need 2 arguments, int START and int END.");
	}

	int start = atoi(argv[1]);
	int end = atoi(argv[2]);

	// -------------- CONSTRUCTION DU GRAPH ----------------
	GraphInfo * gInfo = create_correspondance();

	Graph *g = create_graph(gInfo);

	create_way(gInfo, g);

	//---------- affiche la liste de correspondance entre ids des sommet du graphs et ids du .osm  ------------
	
	
	vector_print(gInfo->correspondance);

	// -------------affiche les lat et lon enregistré dans gInfo->pos ---------------------
	Couple_list* cpl = gInfo->pos;
	


	// -------------affiche le todot du graph ---------------------
	char * td = todot(g);
	printf("%s\n",td);


	//-------trouver le plus court chemins----------
	
	double* dist = calloc(g->order,sizeof(double));
	int* pred = calloc( g->order,sizeof(int));

	Dijkstra(g,start,gInfo,dist,pred);

	
	/*for (int i = 0; i < g->order; ++i)
	{
		printf("distance[i]  = %f Km \n", dist[i]);
	}
	for (int i = 0; i < g->order; ++i)
	{
		printf("pred[%d]  = %d \n",i, pred[i]);
	}*/

	struct List* way = initlist();

	int i=0;
	while (cpl != NULL){
		if (i==start || i == end){
			printf("idx : %i, lat, lon : %f, %f\n",i, cpl->couple->x,cpl->couple->y);
		}
		cpl=cpl->next;
		i++;
	}


	double distance = get_min_way(start,end,pred,dist,way);

	//append(way,10);
	//append(way,7);
	//append(way,4);
	//append(way,8);
	//afficherListe(way);

	printf("Distance entre le sommet %d et %d= %f\n",start,end,distance );


	printGraph(gInfo);
	free(dist);
	free(pred);
	
	// --------------free--------------

	free(td);
	freeGraphInfo(gInfo);
	freeGraph(g);


}