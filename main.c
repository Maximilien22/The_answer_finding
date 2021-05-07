#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <err.h>
#include "parsing.h"
#include "tools.h"
#include "path.h"
#include "vector.h"


int main (){

	// -------------- CONSTRUCTION DU GRAPH ----------------
	GraphInfo * gInfo = create_correspondance();

	Graph *g = create_graph(gInfo);

	create_way(gInfo, g);

	//---------- affiche la liste de correspondance entre ids des sommet du graphs et ids du .osm  ------------
	
	
	vector_print(gInfo->correspondance);

	// -------------affiche les lat et lon enregistré dans gInfo->pos ---------------------
	Couple_list* cpl = gInfo->pos;
	while (cpl != NULL){
		printf("lat : %f, lon : %f\n",cpl->couple->x,cpl->couple->y);
		cpl=cpl->next;
	}


	// -------------affiche le todot du graph ---------------------
	char * td = todot(g);
	printf("%s\n",td);


	//-------trouver le plus court chemins----------
	
	double* dist = calloc(g->order,sizeof(double));
	int* pred = calloc( g->order,sizeof(int));

	Dijkstra(g,3,gInfo,dist,pred);

	
	for (int i = 0; i < g->order; ++i)
	{
		printf("distance[i]  = %f Km \n", dist[i]);
	}
	for (int i = 0; i < g->order; ++i)
	{
		printf("pred[%d]  = %d \n",i, pred[i]);
	}

	

	free(dist);
	free(pred);
	
	// --------------free--------------

	free(td);
	freeGraphInfo(gInfo);
	freeGraph(g);

}