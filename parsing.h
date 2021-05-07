#ifndef H_PARSING
#define H_PARSING

#include "tools.h"
#include "vector.h"

//---------GRAPH INFO----------------
typedef struct GraphInfo GraphInfo;
struct GraphInfo
{
	vector* correspondance;// tableau de correspondance entre id des noeuds et index dans notre graphe
	Couple_list * pos; // liste qui contient lat et lon, pr le i eme sommet
};

GraphInfo* iniGraphInfo();
GraphInfo * create_correspondance();
Graph* create_graph(GraphInfo* gInfo);
GraphInfo * create_way(GraphInfo * gInfo, Graph* G);
void freeGraphInfo(GraphInfo * gInfo);

#endif
