#ifndef H_TOOLS
#define H_TOOLS


//---------COUPLE -----------------------
typedef struct Couple Couple;
struct Couple
{	
	float x;
	float y;
	      
};
Couple *iniCouple (float x, float y);

//---------COUPLE LIST----------------
typedef struct Couple_list Couple_list;
struct Couple_list
{
	   Couple* couple;
	   Couple_list *next;
};


//---------VALUE LIST----------------
typedef struct Value_list Value_list;
struct Value_list
{
	   int value;
	   Value_list *next;
};

Couple_list *iniCoupleList();
Value_list *iniValueList();

//int length(Couple_list *list);

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
	Value_list** adjlists;
	//Couple_list* pos;
	// un name pr charque node
	      
};
Graph *iniGraph(int order);
void addEdge(Graph* G,int s1,int s2);
void freeCoupleList(Couple_list *Cpl);
void freeValueList(Value_list *Vlist);
void freeGraph(Graph* G);
char* todot(Graph *G);


#endif
