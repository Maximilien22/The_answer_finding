
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <err.h>
#include <math.h>
#include "tools.h"
#include "parsing.h"


#define pi 3.14159265358979323846

Graph *iniGraph (int order)
{
	// our graph is not directed

	Graph *graph = malloc(sizeof(Graph));
 
	graph->order = order;
	graph->adjlists = calloc(graph->order, sizeof(Value_list));
	
	//graph->pos = ; // pr le i eme noeud, le i eme element de la liste labels, contient ses coordonées ( un couple de int)
	for(int i =0; i<order; i++)
		graph->adjlists[i] = NULL;
	return graph;
}

void __addOneEdge(Graph* G, int s_from, int s_to){

	Value_list ** cp;
	cp = &(G->adjlists[s_from]);

	Value_list* new_list = iniValueList();
	new_list->value = s_to;
	//new_list->next = NULL;

	if (*cp == NULL){
		 // TODO ca ca marche pas je crois, pck ca verifie le pointeur pas que la valeur = 00000000
		*cp = new_list;
	}else{
		Value_list* curr = *cp;
		while ( curr->next!= NULL){
			curr = curr->next;
		}
		curr->next = new_list;
	}

}

void addEdge(Graph* G,int s1,int s2)
{
	if(s1 == -1 || s2 == -1)
		return;
	
	__addOneEdge(G, s1, s2);
	if (s1 != s2)
		__addOneEdge(G, s2, s1);

}


char* todot(Graph *G){
	
    char* link = " -- ";
    char * dot = calloc (1000000,sizeof(char));

    strcat(dot, "graph {\n");
        

    for (int s =0; s<G->order; s++){
    	char* txt;
    	asprintf(&txt, "  %i",s);
    	strcat(dot, txt);
    	free(txt);
    	//(dot,"[label = \"");
    	//strcat(dot, G->labels[s]);
    	//strcat(dot, "]\n");

    	strcat(dot, "\n");
        

        Value_list * currElement = G->adjlists[s];
        while (currElement != NULL){
        	int adj = currElement->value;

            if  (adj <= s ){ // pr pas dessiner 2 fois le meme arc
            	char* txt2;
    			asprintf(&txt2, "%i",s);
    			strcat(dot, txt2);
    			free(txt2);
            	strcat(dot,link);
     
   
            	char* txt1;
    			asprintf(&txt1, "%i",adj);
            	strcat(dot,txt1);
            	free(txt1);
            	strcat(dot, "\n");
            }
            currElement = currElement->next;
        }

    }
    strcat(dot, "}");

    return dot;

}


Couple *iniCouple (double x, double y)
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
	list->couple = iniCouple(-1, -1);

	return list;
}

Value_list* iniValueList()
{
	Value_list *list = malloc(sizeof(Value_list));
	
	list->next = NULL;
	list->value = -1;

	return list;
}

void addToCoupleList(Couple_list* cpl, Couple* cp){

	if (cpl->couple == NULL || ( cpl->couple->x == -1 && cpl->couple->y == -1 ) ) {
		cpl->couple = cp;
		return;
	}

	Couple_list* cpl_1 = iniCoupleList();
	cpl_1->couple = cp;

	while (cpl->next != NULL){
		cpl=cpl->next;
	}

	cpl->next = cpl_1;

}



int length(Value_list *list){
	if (list==NULL || list->value == 0)
		return 0;
	else
	{
		int len = 0;
		Value_list* currElement = list;

		while (currElement!=NULL){
			len++;
			currElement = currElement->next;
		}
		return len;
	}

}

void freeValueList(Value_list *Cpl)
{
	if(Cpl == NULL)
		err(1,"Error while trying to free Value_list");

	if (Cpl->next != NULL){
		freeValueList(Cpl->next);
	}
	free(Cpl);
}

void freeCoupleList(Couple_list *Cpl)
{
	if(Cpl == NULL)
		err(1,"Error while trying to free Couple_list");

	if (Cpl->next != NULL){
		freeCoupleList(Cpl->next);
	}
	free(Cpl->couple);
	free(Cpl);

}

void freeGraph(Graph* G)
{
 	for(int i = 0; i<G->order; i++)
 	{
 		if(G->adjlists[i] != NULL)
			freeValueList(G->adjlists[i]);
	}
	free(G->adjlists);
	//for (int i=0; i<G->order; i++){
		//free(G->pos[i]);
	//}
	//free(G->pos);
	free(G);
}


double deg2rad(double deg)
{
  return (deg * pi / 180);
}

double rad2deg(double rad)
{
  return (rad * 180 / pi);
}



/*
int main(){ 
	Graph * g = iniGraph(3);
	g->pos[0] = iniCouple(545454,84848);
	g->pos[1] = iniCouple(54,0.55);
	g->pos[2] = iniCouple(-51.5,2652);
	addEdge(g, 0,1);
	addEdge(g, 2,2);
	addEdge(g, 2,1);
	char* td = todot(g);
	printf("%s\n",td);
	free(td);
	freeGraph(g);
}*/