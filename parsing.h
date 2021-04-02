#ifndef H_TOOLS
#define H_TOOLS

#include "tools.h"

//---------GRAPH INFO----------------
typedef struct GraphInfo GraphInfo;
struct GraphInfo
{
	int g_order = 0;
	unsigned short * correspondance;// tableau de correspondance entre id des noeuds et index dans notre graphe
	Couple * pos; // tableau qui contient lat et lon, pr le i eme sommet
};

GraphInfo* iniGraphInfo();



#endif
