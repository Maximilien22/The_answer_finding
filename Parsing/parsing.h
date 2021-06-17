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


void build_Graph_GraphInfo(GraphInfo ** gInfo, Graph **g, char** file_name);
void freeGraphInfo(GraphInfo * gInfo);

#endif
