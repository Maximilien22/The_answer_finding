#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "tools.h"

Graph *iniGraph (int order)
{
	// our graph is not directed

	Graph *graph = malloc(sizeof(Graph));
 
	graph->order = order;
	graph->adjlists = calloc(graph->order, sizeof(Couple_list));

	
	graph->pos = malloc(sizeof(Couple) * graph->order); // pr le i eme noeud, le i eme element de la liste labels, contient ses coordonées ( un couple de int)

	return graph;
}

void __addOneEdge(Graph* G, int s_from, int s_to){

	Couple_list * cp = G->adjlists[s_from];

	Couple_list* new_list = iniCoupleList();
	new_list->value = s_to;
	new_list->next = NULL;

	if (cp == NULL){ // TODO ca ca marche pas je crois, pck ca verifie le pointeur pas que la valeur = 00000000
		cp = new_list;
	}else{
		while ( cp->next!= NULL){
			cp = cp->next;
		}
		cp->next = new_list;
	}

}

void addEdge(Graph* G,int s1,int s2)
{
	__addOneEdge(G, s1, s2);
	if (s1 != s2)
		__addOneEdge(G, s2, s1);

}


//TODO : faire load
//TODO : faire les free
char* todot(Graph *G){
	
    char* link = " -- ";
    char * dot = malloc ( 100000000 );//TODO ca c'est pas bien

    strcat(dot, "graph {\n");
        

    for (int s =0; s<G->order; s++){
    	char* txt;
    	asprintf(&txt, "  %i",s);

    	strcat(dot, txt);
    	//(dot,"[label = \"");
    	//strcat(dot, G->labels[s]);
    	//strcat(dot, "]\n");

    	strcat(dot, "\n");
        

        Couple_list * currElement = G->adjlists[s];
        while (currElement != NULL){
        	int adj = currElement->value;

            if  (adj <= s ){ // pr pas dessiner 2 fois le meme arc
            	char* txt;
    			asprintf(&txt, "%i",s);
    			strcat(dot, txt);
            	strcat(dot,link);
     
   
            	char* txt1;
    			asprintf(&txt1, "%i",adj);
            	strcat(dot,txt1);
            	strcat(dot, "\n");
            }
            currElement = currElement->next;
        }

    }

    strcat(dot, "}");

    return dot;

}


Couple *iniCouple (float x, float y)
{

	Couple *couple = malloc(sizeof(Couple));
 
	couple->x = x;
	couple->y = y;

	return couple;
}


Couple_list* iniCoupleList()
{
	Couple_list *list = malloc(sizeof(Couple_list));
	
	list->next = NULL;
	list->value = -1;

	return list;
}


int length(Couple_list *list){
	if (list==NULL || list->value == 0)
		return 0;
	else
	{
		int len = 0;
		Couple_list* currElement = list;

		while (currElement!=NULL){
			len++;
			currElement = currElement->next;
		}
		return len;
	}

}


/*int main(){ 

	Graph * g = iniGraph(3);

	//g->pos[0] = iniCouple(545454,84848);
	//g->pos[1] = iniCouple(54,0.55);
	//g->pos[2] = iniCouple(-51.5,2652);

	//addEdge(g, 0,1);
	//addEdge(g, 2,2);

	printf("%s\n",todot(g));
}*/
