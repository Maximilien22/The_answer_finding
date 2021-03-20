#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tools.h"

Graph *iniGraph (int order)
{
	// our graoh is not directed

	Graph *graph = malloc(sizeof(*graph));
 
	graph->order = order;
	graph->adjlists = malloc(sizeof(Couple_element*) * (graph->order) );

	for (int i = 0; i < (graph->order); i++)
	{
		graph->adjlists[i] = iniCoupleList();
	}

	graph->pos = malloc(sizeof(Couple*) * graph->order); // pr le i eme noeud, le i eme element de la liste labels, contient ses coordonées ( un couple de int)

	return graph;
}


char* todot(Graph *G){

	
    char* link = " -- ";
    char* dot = "graph {\n";
        

    for (int s =0; s<G->order; s++){
    	char* txt;
    	sprintf(txt, "  %i",s);
    	strcat(dot, txt);
    	//(dot,"[label = \"");
    	//strcat(dot, G->labels[s]);
    	//strcat(dot, "]\n");
    	strcat(dot, "\n");
        
        Couple_element * currElement = G->adjlists[s];
        while (currElement != NULL){
        	int adj = currElement->value;

            if  (adj <= s ){ // pr pas dessiner 2 fois le meme arc
            	char* txt;
    			sprintf(txt, "%i",s);
    			strcat(dot, txt);
            	strcat(dot,link);

            	char* txt1;
    			sprintf(txt1, "%i",adj);
            	strcat(dot,txt1);
            	strcat(dot, "\n");
            }
            currElement = currElement->next;
        }
    }

    strcat(dot, "}");

    return dot;

}


Couple *iniCouple (int x, int y)
{

	Couple *couple = malloc(sizeof(Couple));
 
	couple->x = x;
	couple->y = y;

	return couple;
}


Couple_element* iniCoupleList()
{
	Couple_element *list = malloc(sizeof(Couple_element));
	
	list->next = NULL;
	list->value = 0;

	return list;
}


int length(Couple_element *list){
	if (list==NULL || list->value == 0)
		return 0;
	else
	{
		int len = 0;
		Couple_element* currElement = list;

		while (currElement!=NULL){
			len++;
			currElement = currElement->next;
		}
		return len;
	}

}

