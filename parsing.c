#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <err.h>
#include <math.h>
#include <regex.h>

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

char* file_name = NULL;


int findString(char* str_request, char* str_regex  ){

   int err;
   regex_t preg;

   err = regcomp (&preg, str_regex, REG_EXTENDED);
   if (err == 0)
   {
      int match;
      size_t nmatch = 0;
      regmatch_t *pmatch = NULL;
      
      nmatch = preg.re_nsub;
      pmatch = malloc (sizeof (*pmatch) * nmatch);
      if (pmatch != NULL)
      {
         match = regexec (&preg, str_request, nmatch, pmatch, 0);
         regfree (&preg);
		 free(pmatch);
         return !match;
      }
      else
      {
         fprintf (stderr, "memory error in regex\n");
         exit (EXIT_FAILURE);
      }
   }

	return -1;
}


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

void creates_edges(GraphInfo * gInfo, Graph* G, unsigned long* tab, int size, unsigned char isLit,unsigned char isNotLit ){
	
	
	for(int i=0;i<size-1;i++){
		int id = getCorrespondingId(G->order, gInfo->correspondance, tab[i]);
		G->lit[id] = G->lit[id] || isLit;
		
		G->notLit[id] = G->notLit[id] || isNotLit ;
		addEdge(G, id, getCorrespondingId(G->order, gInfo->correspondance, tab[i+1]) );
	}
}

GraphInfo * create_way(GraphInfo * gInfo, Graph* G){

	FILE * fichier;
	fichier = fopen(file_name, "r");
	//int nodes_of_way_size = 1000;//TODO faire des allocation dynamique

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
	vector * nodes_of_way = vector_new(); // tableau qui contient tout les noeuds d'une way
	
	unsigned char isLit = 0;
	unsigned char isNotLit = 0;
	char* wayTxt = calloc(1,sizeof(char));
	size_t wayTxtMaxSize = 1;
	size_t wayTxtSize = 1;
	

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
				
				wayTxtSize+=1;
				if (wayTxtSize>= wayTxtMaxSize)
					wayTxt = realloc(wayTxt,++wayTxtMaxSize);
				
				
				strncat(wayTxt, &c,1);
				
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
						
						vector_add(nodes_of_way,currRef);
		//				idx_nodes_of_way++;

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
						isLit = findString(wayTxt,"k=\"lit\" v=\"yes\"");
						isNotLit = findString(wayTxt,"k=\"building\"");
						
						//printf("\nisLit : %i\n",isLit);
						creates_edges(gInfo, G,  nodes_of_way->data, nodes_of_way->size, isLit, isNotLit);
						vector_reset(nodes_of_way);
						
						
						memset(wayTxt,0	,wayTxtSize);
						wayTxtSize = 0;						
					
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

	vector_free(nodes_of_way);
	free(wayTxt);
	return gInfo;
}


Graph* create_graph(GraphInfo* gInfo){
	Graph * G = iniGraph(gInfo->correspondance->size);
	return G;
}

void build_Graph_GraphInfo(GraphInfo ** gInfo, Graph **g, char** file_n){
	file_name = *file_n;
	
	*gInfo = create_correspondance();
	*g = create_graph(*gInfo);
	create_way(*gInfo, *g);
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


//TODO : verifier que les users n'ai pas de < ou de =