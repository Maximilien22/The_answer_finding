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
	   Couple_list *end;
};


//---------VALUE LIST----------------
typedef struct Value_list Value_list;
struct Value_list
{
	   int value;
	   Value_list *next;
	   Value_list *end;
};

Couple_list *iniCoupleList();
void addToCoupleList(Couple_list* Cpl, Couple* cp);
void addToValueList(Value_list* value_l, int newVal);
Couple* getFromCoupleList(Couple_list* Cpl, int idx);
Value_list *iniValueList();

//---------GRAPH----------------

typedef struct Graph Graph;// now, rather that typing 'struc Graph' every time, we can just say 'Graph'
struct Graph
{	
	int order;
	Value_list** adjlists;
	unsigned char* lit;
	unsigned char* notLit;
	char** nodeNames;
	int* nodeNameID;
	Value_list** hash_table;
};


typedef struct DoubleInt DoubleInt;
struct DoubleInt
{	
	int x;
	int y;	      
};

Graph *iniGraph(int order);
void addEdge(Graph* G,int s1,int s2);
void freeCoupleList(Couple_list *Cpl);
void freeValueList(Value_list *Vlist);
void freeGraph(Graph* G);
void freeHashTable(Graph* G);
//char* todot(Graph *G);
double deg2rad(double deg);
double rad2deg(double rad);
Couple_list* getPos(Couple_list * pos, int x);
int hash(unsigned long file_id, Graph* g);

#endif
