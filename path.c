#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <err.h>
#include <math.h>
#include "tools.h"
#include "parsing.h"

//Get x = longitude and y = latitude of a edge.
void get_data(GraphInfo* gInfo, int i ,double* x, double* y)
{
	Couple_list* cpl = gInfo->pos;
	for (int j = 0; j < i; ++j){
		printf("%d\n", i);
		cpl = cpl->next;
	}
	*x = cpl->couple->x;
	*y = cpl->couple->y; 
}

//Get doc on get_distance on :
//https://www.geodatasource.com/developers/c
double get_distance(double lat1, double lon1, double lat2, double lon2)
{
	double dist;
  	if ((lat1 == lat2) && (lon1 == lon2))
    	return 0;

    dist = sin(deg2rad(lat1)) * sin(deg2rad(lat2)) + cos(deg2rad(lat1)) * cos(deg2rad(lat2)) * cos(deg2rad(lon1 - lon2));
   	dist = acos(dist);
   	dist = rad2deg(dist);
   	dist = dist * 60 * 1.1515;
    return dist * 1.609344;
}

double cost(Graph* G, GraphInfo* gInfo, int s1, int s2)
{
	if (s1 == s2)
		return 0;

	Value_list* currElement = G->adjlists[s1];
	int adj;
	double lat1;
	double lon1;
	double lat2;
	double lon2;
	if (currElement != NULL)
	{
		while (currElement != NULL)
		{
			adj = currElement->value;
			if (adj == s2)
			{

				get_data(gInfo,s1,&lat1,&lon1);
				get_data(gInfo,s2,&lat2,&lon2);
				return get_distance(lat1,lon1,lat2,lon2);
			}
			currElement = currElement->next;
		}
	}
	return -1;
}

int is_empty(int* list, int len)
{
	for (int i = 0; i < len; ++i)
	{
		if (list[i] != -1)
			return 0;
	}
	return 1;
}

int heuristic(Graph* G,int* M, double* dist)
{
	int res = -1;
	for (int i = 0; i < G->order; ++i)
	{
		if (M[i] != -1)
		{
			if ((res == -1 || dist[i] < dist[res]) && dist[i] != -1)
			{	
				res = i;
			}
		}
	}
	return res;
}

void Dijkstra(Graph* G, int start, GraphInfo* gInfo, double* res_dist, int* res_pred)
{
	//=====================INIT=======================
	int* M = malloc(sizeof(int)*G->order);
	for (int i = 0; i < G->order; ++i)
	{
		res_dist[i] = cost(G, gInfo, start, i);
	}

	for (int i = 0; i < G->order; ++i)
	{
		*(res_dist + i) = cost(G, gInfo, start, i);
		printf("res_dist[%d] = %f\n",i, res_dist[i]);
	}
	for (int i = 0; i < G->order; ++i)
	{
		res_pred[i] = start;
		printf("res_pred[%d] = %d\n",i, res_pred[i]);
	}

	res_pred[start] = -1;
	M[start] = -1;
	//================================================
	int e;
	Value_list* currElement;
	int adj;
	double Cost;
	//================================================
	while (!is_empty(M, G->order))
	{
		e = heuristic(G, M, res_dist);
		printf("e = %d\n", e);

		for (int i = 0; i < G->order; ++i)
		{
			printf("ditance2[%d] = %f\n", i,res_dist[i]);
		}
		if (e == -1 || e == start) 
			break;
		
		M[e] = -1;

		currElement = G->adjlists[e];
		if (currElement != NULL)
		{
			while (currElement != NULL)
			{
				adj = currElement->value;
				Cost = cost(G, gInfo, e, adj);

				if (res_dist[adj] == -1 || res_dist[e] + Cost < res_dist[adj])
				{
					res_dist[adj] = res_dist[e] + Cost;
					res_pred[adj] = e;
					printf("OUI PROBLEMES AVEC [%d] et [%d]\n",e,adj );
				}
				currElement = currElement->next;
			}
		}
		else
			printf("BONJOUR pour [%d]\n",e );
	}

	free(M);
}
