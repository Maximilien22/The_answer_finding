#define _GNU_SOURCE

#include "parsing.h"
#include "tools.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <err.h>

#define id_done 0
#define lat_done 1
#define lon_done 2

#define reading_id 3
#define t_read 4
#define a_read 5
#define l_read 6
#define n_read 7
#define in_node 8

#define flags_nb 9

#define NB_SOMMETS 10

GraphInfo * iniGraphInfo(){
	GraphInfo * gInfo = malloc(sizeof(GraphInfo));
	
	gInfo->idx = 0;
	gInfo->correspondance = malloc(sizeof(unsigned long)* NB_SOMMETS);
	gInfo->pos = malloc(sizeof(Couple)*NB_SOMMETS);

	return gInfo;

}

int * iniNodeFlags(){
	return calloc(flags_nb, sizeof(int));
}

int isNumber( char c){
	return c>='0' && c<='9';
}

void create_node(char c, GraphInfo* gInfo, int* node_flags, unsigned long* currId){
	if (c=='i' &&  ! node_flags[reading_id] && ! node_flags[id_done] ){
		node_flags[reading_id] = 1;
	}
	if (isNumber(c) && node_flags[reading_id]){
		(*currId) *= 10;
		(*currId) += (c-'0') ;
	}

	if (node_flags[reading_id] && c==' '){
		node_flags[reading_id] = 0;
		node_flags[id_done] = 1;
		gInfo->correspondance[(gInfo->idx)] = *currId;
		(gInfo->idx)+=1;
		*currId = 0;
	}
}


void search_for_node(char c, int* node_flags ){

	if (node_flags[n_read] && !node_flags[in_node]){
		if (c=='o'){
			node_flags[in_node]=1;
			node_flags[n_read] = 0;
		}else{
			node_flags[n_read] = 0;
		}
	}

	if (c=='n' && !node_flags[in_node] ){
		node_flags[n_read]=1;
	}
}
GraphInfo * create_correspondance(){

	FILE * fichier;
	fichier = fopen("epita.osm", "r");
	unsigned long currId = 0; 

	char c = fgetc(fichier);

	//-----GRAPH------
	
	GraphInfo * gInfo = iniGraphInfo();

	//-----searching nodes------
	

	// ------ nodes reading flags -------
	int *node_flags = iniNodeFlags();

	if (fichier != NULL){
									                
		while( !feof(fichier) ) {
			//printf("%c\n",c );
			//printf("1 reading_id %i\n",node_flags[reading_id] );
			//printf("2 n_read %i\n",node_flags[n_read]);
			//printf("3 in_node %i\n",node_flags[in_node]);
			//printf("4 id_done %i\n",node_flags[id_done]);

      		if (! (node_flags[in_node]) ){
      			search_for_node(c, node_flags);
      		}else{
      			create_node(c, gInfo, node_flags, &currId);
      		}

      		if (c=='>' && node_flags[in_node]){
      			node_flags[in_node]=0;
      			node_flags[id_done] = 0;
      			node_flags[n_read]=0;
      		}
      		//printf("%c", c);
      		c = fgetc(fichier);
   		}
		
		fclose(fichier);

	}else{
		err(1,"cant open file");
	}


	return gInfo;
}

void create_edges(Graph* G, GraphInfo* gInfo)
{
	//TODO faire add edge
	//TODO Trouver les ways, lire les noeuds
	//TODO Lire les correspondances et faire des add_edges
}

Graph* create_graph(GraphInfo* gInfo)
{
	Graph * G = iniGraph(gInfo->idx);

}

int main (){

	GraphInfo * gInfo = create_correspondance();

	create_graph(gInfo);

	for (int i=0; i<NB_SOMMETS; i++){
		printf("[%lu] ",gInfo->correspondance[i] );
	}
	printf("\n");
	/*printf("\n\nidx : %s\n",gInfo->idx );
	Graph *g = iniGraph(gInfo->idx);

	printf("%s\n",todot(g));*/

}

//-créé tt les nodes
// tableau de correspondance entre id des noeuds et index dans adjlists

//TODO : faire des allocations dynamique de memoire

// parcourir toutes les ways et connectée les nodes 2 a 2