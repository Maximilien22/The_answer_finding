#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <err.h>
#include "parsing.h"
#include "tools.h"
#include "path.h"
#include "vector.h"
#include "list.h"
#include "graphics.h"
#include "math.h"

char* file = "nearpita.osm";



int main (int argc, char** argv)
{
	
	if (argc != 3)
	{
		err(1,"ERROR argument : need 2 arguments, int START and int END.");
	}

	int start = atoi(argv[1]);
	int end = atoi(argv[2]);
	


	// -------------- CONSTRUCTION DU GRAPH ----------------
	GraphInfo * gInfo = NULL;
	Graph *g = NULL;
	
	build_Graph_GraphInfo(&gInfo, &g, &file );

	double lat1;
	double lat2;
	double long1;
	double long2;

	get_data(gInfo,start,&lat1,&long1);
	get_data(gInfo,end,&lat2,&long2);
	double u_x = long2 - long1;
	double u_y = lat2 - lat1;



	printf("---------V1---------\n");
	printf("%f , %f\n", lat1,long1);
	printf("%f , %f\n", lat2,long2);
	printf("-----||%f||--------||%f||-----\n",u_x,u_y);

	get_data(gInfo,start,&lat1,&long1);
	get_data(gInfo,56,&lat2,&long2);



	double v_x = long2 - long1;
	double v_y = lat2 - lat1;
	printf("---------V2---------\n");
	printf("%f , %f\n", lat1,long1);
	printf("%f , %f\n", lat2,long2);
	double oui1 = 0;
	double oui2 = 0;
	if (v_x * v_y < 0)
	{
		oui2 = 180;
	}
	if (u_x * u_y < 0)
	{
		oui1 = 180;
	}
	printf("arctan de v = %f\n",atan2(v_x,v_y)+ oui2 );
	printf("arctan de u = %f\n", atan2(u_x,u_y) + oui1);
	printf("------||%f||---------||%f||---\n",v_x,v_y);
	double res = (atan2(v_x,v_y)+ oui2) - (atan2(u_x,u_y) + oui1);
	printf("Angle = %f\n", res);
	
    int* pred = calloc(g->order, sizeof(int));
    double* dist = calloc(g->order,sizeof(double));

    struct List* list = initlist();
    A_start(g, gInfo, start, end, dist, pred, 2);
    //Dijkstra(g, start, gInfo, dist, pred);

    double diiiist = get_min_way(start,end,pred,dist,list);
    int a = end;
    while (a != start)
    {
    	if (g->lit[a] == 0)
    	{
    		printf("Ueee chui un malade moi. Jsuis le numero %d \n",a);
    	}
    	a = pred[a];
    }

    afficherListe(list);
    printf("Distance = %f\n", diiiist);
   	


   
	// -------------affiche le todot du graph ---------------------
	//char * td = todot(g);
	//printf("%s\n",td);

	

	//displayMap(gInfo, g );
	
	
	
	/*free(dist);
	free(pred);*/
	
	// --------------free--------------

	//free(td);
	freeGraphInfo(gInfo);
	freeGraph(g);
	
	


}