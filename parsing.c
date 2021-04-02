#define _GNU_SOURCE

#include "tools.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <err.h>

#define id_done 0
#define lat_done 1
#define lon_done 2

#define i_read 3
#define t_read 4
#define a_read 5
#define l_read 6
#define o_read 7
#define n_read 8


GraphInfo * iniGraphInfo(){
	GraphInfo * gInfo = malloc(sizeof(GraphInfo));
	
	gInfo->correspondance = malloc(sizeof(unsigned short)*1 000 000 000);
	gInfo->pos = malloc(sizeof(Couple)*1000);

	return gInfo;

}

int isNumber( char c){
	return c>='0' && c<='9';
}

void create_node(char c, GraphInfo* g_info, *int node_flags){
	if (c=='i' &&  !node_flags[id_done]){
		node_flags[i_read] = 1;
	}
	if (isNumber(c) && !node_flags[id_done] && node_flags[i_read]){
		g_info->correspondance[g_info->g_order]
	}
}


void search_for_node(char c, int *n_read, int *o_read, int *in_node ){

	if (*n_read){
		if (c=='o')
			*o_read =1;
		else{
			*n_read=0;
		}
	}

	if (c=='n' ){
		*n_read=1;
	}

	if (*o_read && *n_read){
		*in_node=1;
	}
}
void create_graph(){

	FILE * fichier;
	fichier = fopen("epita.osm", "r");

	char c = fgetc(fichier);

	//-----GRAPH------
	
	GraphInfo * gInfo = iniGraphInfo();

	//-----searching nodes------
	int n_read = 0;
	int o_read = 0;
	int in_node= 0;
	int search_node =1;

	// ------ nodes reading flags -------
	int *node_flags = calloc(sizeof(int)*9);

	if (fichier != NULL){
									                
		while( !feof(fichier) ) {
      		

      		if (search_node):
      			search_for_node(c, &n_read, &o_read, &in_node);

			if (in_node){
      			create_node(c, gInfo, node_flags);
      		}

      		if (c=='>' && in_node){
      			search_node = 0;
      			in_node=0;
      			n_read=0;
      			o_read=0;
      		}



      		//printf("%c", c);
      		c = fgetc(fichier);
   		}
		
		fclose(fichier);

	}else{
		err(1,"cant open file");
	}

}

int main (){

	create_graph();
}

//-créé tt les nodes
// tableau de correspondance entre id des noeuds et index dans adjlists

// parcourir toutes les ways et connectée les nodes 2 a 2