
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <err.h>
#include <math.h>
#include "parsing.h"
#include "tools.h"
#include "path.h"
#include "vector.h"

#define id_done 0
#define lat_done 1
#define lon_done 2

#define reading_id 3
#define t_read 4
#define a_read 5
#define l_read 6
#define n_read 7
#define in_node 8
#define equal_read 9
#define o_read 10
#define reading_lat 11
#define reading_lon 12
#define coma_read 13
#define chevront_open_read 14

#define flags_nb 15

#define file_name "nearpita.osm"

GraphInfo * iniGraphInfo(){
	GraphInfo * gInfo = calloc(1, sizeof(GraphInfo));
	
	gInfo->correspondance = vector_new();
	gInfo->pos = iniCoupleList();

	return gInfo;
}

int * iniNodeFlags(){
	return calloc(flags_nb, sizeof(int));
}


int isNumber( char c){
	return c>='0' && c<='9';
}

void create_node(char c, GraphInfo* gInfo, int* node_flags, unsigned long* currId, double* currLat, double* currLon, int* divider){
	
	if (node_flags[reading_lat]){
		if (c == '.'){
			node_flags[coma_read]=1;
		}
		else if (isNumber(c)){
			//printf("lat : %c\n", c);
			if (!node_flags[coma_read]){
				(*currLat)*=10;
				(*currLat)+=(c-'0');
			}else{
				(*currLat) += (double) (c-'0')/(*divider);
				(*divider) *=10;

			}

		}else if (c==' '){
			
			//TODO : l'ajouter a GraphInfo
			node_flags[l_read] = 0;
			node_flags[a_read] = 0;
			node_flags[t_read] = 0;
			node_flags[equal_read] = 0;
			node_flags[reading_lat] = 0;
			node_flags[coma_read]= 0;
			*divider =10;
			
		}
	}
	
	if (node_flags[reading_lon]){
		if (c == '.'){
			node_flags[coma_read]=1;
		}else if (isNumber(c)){
			//printf("lon : %c\n",c );
			if (!node_flags[coma_read]){
				(*currLon)*=10;
				(*currLon)+=(c-'0');
			}else{
				(*currLon) += (double) (c-'0')/(*divider);
				(*divider)*=10;
			}

		}else if (c=='/' || c=='>'){
			//printf("lat : %f\n", *currLat);
			//printf("lon : %f\n", *currLon);
			//TODO : l'ajouter a GraphInfo

			Couple* cp = iniCouple(*currLat, *currLon);
			addToCoupleList(gInfo->pos, cp);

			node_flags[l_read] = 0;
			node_flags[o_read] = 0;
			node_flags[n_read] = 0;
			node_flags[equal_read] = 0;
			node_flags[reading_lon] = 0;
			node_flags[coma_read]= 0;
			*divider =10;
			*currLon = 0.0;
			*currLat =0.0;

			
		}
	}

	if (isNumber(c) && node_flags[reading_id]){
		(*currId) *= 10;
		(*currId) += (c-'0');
	}

	if (c=='i' &&  ! node_flags[reading_id] && ! node_flags[id_done] ){
		node_flags[reading_id] = 1;
	}

	else if (node_flags[reading_id] && c==' '){
		node_flags[reading_id] = 0;
		node_flags[id_done] = 1;
		vector_add(gInfo->correspondance, *currId);
		*currId = 0;
	}

	else if (node_flags[t_read]){
		if (c=='='){
			node_flags[reading_lat] = 1;
		}else{
			node_flags[l_read] = 0;
			node_flags[a_read] = 0;
			node_flags[t_read] = 0;
		}

	}

	else if (node_flags[n_read]){
		if (c=='='){
			node_flags[reading_lon] = 1;
		}else{
			node_flags[l_read] = 0;
			node_flags[o_read] = 0;
			node_flags[n_read] = 0;
		}

	}


	else if (node_flags[o_read] ){
		if (c=='n'){
			node_flags[n_read] = 1;
		}else{
			node_flags[o_read] = 0;
			node_flags[l_read] = 0;
		}
	}

	else if (node_flags[a_read] ){
		if (c=='t'){
			node_flags[t_read] = 1;
			//printf("t : \n");
		}else{
			node_flags[l_read] = 0;
			node_flags[a_read] = 0;
		}
	}

	else if (node_flags[l_read]){
		if (c=='o'){
			node_flags[o_read] = 1;
		}else if (c=='a'){
			node_flags[a_read] = 1;

		}else{
			node_flags[l_read] = 0;
		}
	}
	
	else if (c=='l'){
		node_flags[l_read] = 1;
		//printf("l : \n");
	}
	
}


void search_for_node(char c, int* node_flags ){

	if (node_flags[chevront_open_read] && node_flags[n_read] && !node_flags[in_node]){
		if (c=='o'){
			node_flags[in_node]=1;
			
		}

		node_flags[n_read] = 0;
		node_flags[chevront_open_read] = 0;
		
	}else if (node_flags[chevront_open_read] && !node_flags[in_node] ){
		if (c=='n'){
			node_flags[n_read]=1;
		}else{
			node_flags[chevront_open_read] = 0;
		}
		
	} else if (c=='<'){
		node_flags[chevront_open_read] = 1;
	}
}

GraphInfo * create_correspondance(){

	FILE * fichier;
	fichier = fopen(file_name, "r");
	unsigned long currId = 0; 
	double currLat = 0; 
	double currLon = 0; 
	int divider = 10;

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
      			create_node(c, gInfo, node_flags, &currId, &currLat, &currLon, &divider);
      			//printf("%c",c );
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

int getCorrespondingId(int size, vector * correspondance, unsigned long id){
	int i=0; 
	while (i<size){
		if (correspondance->data[i] == id)
			return i;

		i++;
	}
	return -1;
}

void creates_edges(GraphInfo * gInfo, Graph* G, unsigned long* tab, int size){
	
	for(int i=0;i<size-1;i++){
		addEdge(G, getCorrespondingId(G->order, gInfo->correspondance, tab[i]), getCorrespondingId(G->order, gInfo->correspondance, tab[i+1]) );
	}
}

GraphInfo * create_way(GraphInfo * gInfo, Graph* G){

	FILE * fichier;
	fichier = fopen(file_name, "r");
	int nodes_of_way_size = 1000;

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



Graph* create_graph(GraphInfo* gInfo)
{
	Graph * G = iniGraph(gInfo->correspondance->size);
	return G;
}

void freeGraphInfo(GraphInfo * gInfo){

	/*for (int i=0; i<gInfo->idx; i++){
		free(&(gInfo->pos[i]));
	}*/
	//free(gInfo->pos);
	vector_free(gInfo->correspondance);
	freeCoupleList(gInfo->pos);
	free(gInfo);
	
}

//-créé tt les nodes
// tableau de correspondance entre id des noeuds et index dans adjlists
// parcourir toutes les ways et connectée les nodes 2 a 2


//TODO : ajouter les names aux noeuds ( soit direct dans node, ou si dans way, ajouter au premier noeud),
// pr ensuite pouvoir faire un parcours de place danton a restaurant l'inattendu par ex
//TODO : verifier que les users n'ai pas de < ou de =