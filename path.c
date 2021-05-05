#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <err.h>
#include <math.h>
#include "tools.h"
#include "parsing.h"

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

	Couple_list* cpl1 = gInfo->pos;
	Couple_list* cpl2 = gInfo->pos;

	Value_list* currElement = G->adjlists[s1];
	int adj;

	while (currElement != NULL)
	{
		adj = currElement->value;
		if (adj == s2)
		{
			for (int i = 0; i < s1; ++i)
				cpl1 = cpl1->next;
			for (int i = 0; i < s2; ++i)
				cpl2 = cpl2->next;

			return get_distance(cpl1->couple->x, cpl1->couple->y, 
				cpl2->couple->x, cpl2->couple->y);
		}
		currElement = currElement->next;
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
			if (res == -1 || dist[i] < dist[res])
			{
				if (dist[i] != -1)
					res = i;
			}
		}
	}
	return res;
}

void Dijkstra(Graph* G, int start, GraphInfo* gInfo, double* res_dist, int* res_pred)
{
	int* M = malloc(sizeof(int)*G->order);
	for (int i = 0; i < G->order; ++i)
	{
		M[i] = i;
		res_dist[i] = cost(G, gInfo, start, i);
	}

	for (int i = 0; i < G->order; ++i)
	{
		res_pred[i] = start;
		printf("dist init = %f\n", res_dist[i]);
	}
	res_pred[start] = -1;

	M[start] = -1;
	int e;
	Value_list* currElement;
	int adj;
	double Cost;
	while (!is_empty(M, G->order))
	{
		e = heuristic(G, M, res_dist);

		if (e == -1 || e == start) 
			break;
		
		M[e] = -1;

		currElement = G->adjlists[e];
		while (currElement != NULL)
		{
			printf("couple(%d, %d)\n", e,adj);
			adj = currElement->value;
			Cost = cost(G, gInfo, e, adj);

			if (res_dist[adj] == -1 || res_dist[e] + Cost < res_dist[adj])
			{
				res_dist[adj] = res_dist[e] + Cost;
				res_pred[adj] = e;
				printf("Ah ! couple(%d, %d))\n",e,adj);
			}
			currElement = currElement->next;
		}
	}

	free(M);
}
