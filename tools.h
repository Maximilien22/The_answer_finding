#ifndef H_TOOLS
#define H_TOOLS



//---------COUPLE -----------------------
typedef struct Couple Couple;
struct Couple
{	
	double x;
	double y;
	      
};
Couple *iniCouple (double x, double y);

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
void addToCoupleList(Couple_list* Cpl, Couple* cp);
Couple* getFromCoupleList(Couple_list* Cpl, int idx);
Value_list *iniValueList();

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
double deg2rad(double deg);
double rad2deg(double rad);

#endif
