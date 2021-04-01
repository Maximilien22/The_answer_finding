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
	graph->adjlists = malloc(sizeof(Couple_list) * (graph->order) );

	for (int i = 0; i < (graph->order); i++)
	{
		graph->adjlists[i] = iniCoupleList();
	}

	graph->pos = malloc(sizeof(Couple) * graph->order); // pr le i eme noeud, le i eme element de la liste labels, contient ses coordonées ( un couple de int)

	return graph;
}


char* todot(Graph *G){
	
    char* link = " -- ";
    char * dot = malloc ( 100000000 );

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
	list->value = 0;

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


int main(){ 

	Graph * g = iniGraph(3);

	g->pos[0] = iniCouple(545454,84848);
	g->pos[1] = iniCouple(54,0.55);
	g->pos[2] = iniCouple(-51.5,2652);


	Couple_list* l6 =iniCoupleList();
	l6->next = NULL;
	l6->value = 1;

	Couple_list* l =iniCoupleList();
	l->next = l6;
	l->value = 2;
	g->adjlists[0] = l;

	Couple_list* l4 =iniCoupleList();
	l4->next = NULL;
	l4->value = 0;
	Couple_list* l1 =iniCoupleList();
	l1->next = l4;
	l1->value = 2;

	g->adjlists[1] = l1;


	Couple_list* l5 =iniCoupleList();
	l5->next = NULL;
	l5->value = 1;

	Couple_list* l2 =iniCoupleList();
	l2->next = l5;
	l2->value = 0;
	g->adjlists[2] = l2;

	printf("%s\n",todot(g));
}
