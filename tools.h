#ifndef H_TOOLS
#define H_TOOLS

typedef struct Couple Couple;
struct Couple
{	
	int x;
	int y;
	      
};


typedef struct Couple_element Couple_element;
struct Couple_element
{
	   int value;
	   Couple_element *next;
};

Couple_element *iniCoupleList();
int length(Couple_element *list);
/*float float_get(Float_list *list, int index);
void float_set(Float_list *list, int index, float value);
void float_append(Float_list *list, float nb);
void float_pop(Float_list *list);
void float_remove(Float_list *list, int index);
void float_add(Float_list *list,float nb, int index);
void float_printList(Float_list *list);*/


typedef struct Graph Graph;// now, rather that typing 'struc Graph' every time, we can just say 'Graph'
struct Graph
{	
	int order;
	Couple_element** adjlists;
	Couple* pos;
	      
};
Graph *iniGraph(int order);
char* todot(Graph *G);


#endif
