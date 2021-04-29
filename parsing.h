#ifndef H_PARSING
#define H_PARSING

#include "tools.h"

//---------GRAPH INFO----------------
typedef struct GraphInfo GraphInfo;
struct GraphInfo
{
	int idx; // = G.order quand le graph est fini d'etre créé
	unsigned long * correspondance;// tableau de correspondance entre id des noeuds et index dans notre graphe
	Couple_list * pos; // liste qui contient lat et lon, pr le i eme sommet
};

GraphInfo* iniGraphInfo();



#endif
