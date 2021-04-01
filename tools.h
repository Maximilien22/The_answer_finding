#ifndef H_TOOLS
#define H_TOOLS


//---------COUPLE -----------------------
typedef struct Couple Couple;
struct Couple
{	
	int x;
	int y;
	      
};
Couple *iniCouple (float x, float y);

//---------COUPLE LIST----------------
typedef struct Couple_list Couple_list;
struct Couple_list
{
	   int value;
	   Couple_list *next;
};

Couple_list *iniCoupleList();
int length(Couple_list *list);
/*float float_get(Float_list *list, int index);
void float_set(Float_list *list, int index, float value);
void float_append(Float_list *list, float nb);
void float_pop(Float_list *list);
void float_remove(Float_list *list, int index);
void float_add(Float_list *list,float nb, int index);
void float_printList(Float_list *list);*/

//---------GRAPH----------------

typedef struct Graph Graph;// now, rather that typing 'struc Graph' every time, we can just say 'Graph'
struct Graph
{	
	int order;
	Couple_list** adjlists;
	Couple** pos;
	      
};
Graph *iniGraph(int order);
char* todot(Graph *G);


#endif
