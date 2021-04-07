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
	//gInfo->pos = malloc(sizeof(Couple)*NB_SOMMETS);

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
		
		free(node_flags);
		fclose(fichier);

	}else{
		err(1,"cant open file");
	}

	return gInfo;
}

int getCorrespondingId(int size, unsigned long * correspondance, unsigned long id){
	int i=0; 
	while (i<size){
		if (correspondance[i] == id)
			return i;

		i++;
	}
}

void creates_edges(GraphInfo * gInfo, Graph* G, unsigned long* tab, int size){
	
	for(int i=0;i<size-1;i++){
		addEdge(G, getCorrespondingId(G->order, gInfo->correspondance, tab[i]), getCorrespondingId(G->order, gInfo->correspondance, tab[i+1]) );
	}
}

GraphInfo * create_way(GraphInfo * gInfo, Graph* G){

	FILE * fichier;
	fichier = fopen("epita.osm", "r");
	int nodes_of_way_size = 100;

	char c = fgetc(fichier);

	
	// check if in way
	int chevrons_open_read = 0;

	
	int inWay = 0;

	// ref stuff
	int f_read = 0;
	int readingRef = 0;
	unsigned long currRef = 0; 

	// check if out of way
	int y_read =0;
	unsigned long * nodes_of_way = calloc(nodes_of_way_size, sizeof(unsigned long)); // tableau qui contient tout les noeuds d'une way
	int idx_nodes_of_way = 0;

	if (fichier != NULL){

		while( !feof(fichier) ) {
			
			if (! inWay){// trying to find a way
				if (c=='<'){
					chevrons_open_read = 1;
				}
				else if (chevrons_open_read){
					if (c=='w'){
						inWay = 1;
					}
					else{
						chevrons_open_read = 0;
					}
				}
			}else{ // we are in a way,  trying to find the end of the way
				
				if (! readingRef){ // we are trying to find a ref=
					if (c=='f'){
						f_read = 1;
					}else if (f_read){
						if (c=='='){
							readingRef = 1;
						}
						else{
							f_read = 0;
						}
					}
				}else{ // we are trying to find end of ref=
					if (isNumber(c)){
						currRef*=10;
						currRef += (c-'0');
					}
					else if (c=='/'){// end of ref
						
						nodes_of_way[idx_nodes_of_way] = currRef;
						idx_nodes_of_way++;

						//printf("j'ajoute : %lu a mon tab\n",currRef );
						currRef = 0;
						readingRef=0;
					}
				}

				if (c=='y'){
					y_read = 1;
				}else if (y_read){
					if (c=='>'){// end of way
						inWay = 0;
						creates_edges(gInfo, G,  nodes_of_way, idx_nodes_of_way);
						free(nodes_of_way);
						nodes_of_way = calloc(nodes_of_way_size, sizeof(unsigned long));
						idx_nodes_of_way = 0;
						// reinitialise our nodes of way, beacause end of way
					
					}
					else{
						y_read = 0;
					}
				}
			}

      		

      		c = fgetc(fichier);
   		}
		
		fclose(fichier);

	}else{
		err(1,"cant open file");
	}

	free(nodes_of_way);
	return gInfo;
}

void create_edges(Graph* G, GraphInfo* gInfo)
{
	// faire add edge
	// Trouver les ways, lire les noeuds
	// Lire les correspondances et faire des add_edges
}

Graph* create_graph(GraphInfo* gInfo)
{
	Graph * G = iniGraph(gInfo->idx);
}

void freeGraphInfo(GraphInfo * gInfo){

	/*for (int i=0; i<gInfo->idx; i++){
		free(&(gInfo->pos[i]));
	}*/
	//free(gInfo->pos);
	free(gInfo->correspondance);
	free(gInfo);
	
}


int main (){

	GraphInfo * gInfo = create_correspondance();

	Graph *g = create_graph(gInfo);

	create_way(gInfo, g);

	for (int i=0; i<NB_SOMMETS; i++){
		printf("[%lu] ",gInfo->correspondance[i] );
	}
	printf("\n");
	
	printf("\n\nidx : %i\n",gInfo->idx);

	char * td = todot(g);
	printf("%s\n",td);
	free(td);

	freeGraphInfo(gInfo);
	freeGraph(g);

}

//-créé tt les nodes
// tableau de correspondance entre id des noeuds et index dans adjlists
// parcourir toutes les ways et connectée les nodes 2 a 2

//TODO : faire des allocations dynamique de memoire
//TODO allocation dynamique dans todot
//TODO : cree les nodes avec pos et adapter les free
//TODO : verifier que les users n'ai pas de < ou de =