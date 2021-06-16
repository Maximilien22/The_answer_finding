#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <err.h>
#include <math.h>
#include "list.h"
#include "tools.h"
#include "parsing.h"
#include "heap.h"

//Get x = longitude and y = latitude of a edge.
void get_data(Couple** positions, int i ,double* x, double* y)
{
	*x = (*positions)[i].x;
	*y = (*positions)[i].y; 
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

double cost(Graph* G, Couple** positions, int s1, int s2)
{
	if (s1 == s2)
		return 0;

	Value_list* currElement = G->adjlists[s1];
	int adj;
	double lat1;
	double lon1;
	double lat2;
	double lon2;

	while (currElement != NULL)
	{
		adj = currElement->value;
		if (adj == s2)
		{

			get_data(positions,s1,&lat1,&lon1);
			get_data(positions,s2,&lat2,&lon2);
			return get_distance(lat1,lon1,lat2,lon2);
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

int get_next(Graph* G,int* M, double* dist)
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

void Dijkstra(Graph* G, int start, Couple** positions, double* res_dist, int* res_pred)
{
	//=====================INIT=======================
	int* M = calloc(G->order,sizeof(int));

	for (int i = 0; i < G->order; i++)
	{
		M[i] = 1;
		res_pred[i] = -1;
		res_dist[i] = -1;
	}

	res_dist[start] = 0;

	//================================================
	int e;
	Value_list* currElement;
	int adj;
	double Cost;
	//================================================
	while (!is_empty(M, G->order))
	{
		e = get_next(G, M, res_dist);
		if (e == -1) 
			break;
		
		M[e] = -1;

		currElement = G->adjlists[e];
		if (currElement != NULL)
		{
			while (currElement != NULL)
			{
				adj = currElement->value;
				Cost = cost(G, positions, e, adj);
				if (res_dist[adj] == -1 || res_dist[e] + Cost < res_dist[adj])
				{
					res_dist[adj] = res_dist[e] + Cost;
					res_pred[adj] = e;
				}
				currElement = currElement->next;
			}
		}
	}

	free(M);
}

double get_min_way(int start, int end,int* pred,double* dist,struct List* way)
{
	if (dist[end] == -1){
		return -1;
	}

	int i = end;
	while (i != start)
	{	
		append(way,i);
		i = pred[i];
	}
	append(way, start);
	afficherListe(way);

	return dist[end];
}


double get_angle(Couple** positions, int start, int s)
{
	double lat1;
	double long1;
	get_data(positions,s,&lat1,&long1);
	double u_x = lat1;
	double u_y = long1;
	get_data(positions,start,&lat1,&long1);
	u_x -= lat1;
	u_y -= long1;
	double res = atan2(u_x,u_y);
	if (u_x * u_y < 0)
		res += 180;
	return res;
}

void A_star_Heuristique(Graph* G, Couple** positions, double* Heuristic, double angle_end, int start)
{
	double lat1;
	double long1;
	double lat2;
	double long2;
	double angle;
	get_data(positions, start, &lat1, &long1);

	for (int i = 0; i < G->order; ++i)
	{
		get_data(positions, i, &lat2, &long2);

		if ((lat2 - lat2) * (long2 - long1) < 0)
		angle = (angle_end - (atan2(long2-long1,lat2-lat1) + 180)) * 0.31830988618;
		else
		{
			angle = (angle_end - atan2(long2-long1,lat2-lat1)) * 0.31830988618;
		}

		Heuristic[i] += get_distance(lat1,long1,lat2,long2) * angle * 2;
	}
}

void A_star(Graph* G,Couple** positions, int start, int end, double* res_dist, int* res_pred, unsigned char want_lighted) // si Want_lighted == 0 , veut full light
{
	//struct List* h = initlist();

	int iter = 0;				//number of iteration.
	double oui;					//trash of cost heap on pop
	int adj;					//adjacent of e
	double new_cost;			//new cost
	Value_list* currElement;	//Use for adjLists

	 	
	double* heur = calloc(G->order, sizeof(double));	//Heuristic.
	int* M = calloc(G->order,sizeof(int));				//Put True if the element have been traited.
	struct heap* h = initheap();						//Init Heap


	for (int i = 0; i < G->order; ++i)			//Init all vectors.
	{
		res_dist[i] = -1;
		res_pred[i] = -1;
		heur[i] = 0;
		M[i] = 0;
 	}

 	A_star_Heuristique(G,positions,heur,get_angle(positions, start, end),start); 	//Updating the heuristic list.

 	res_dist[start] = 0;
 	//update(h,start,0);
 	heap_update(h,start, 0);
 	int e;
 	while (!heap_isempty(h))
 	{
 		heap_pop(h,&e,&oui);
 		//h_pop(h,&e,&oui);
 		iter++;
 		if (e == -1)
 			errx(1,"Destination not reachable.\n");

 		if (e == end)
 		{
 			free(heur);

 			free(M);
 			printf("iter - %d\n",iter );
 			return;
 		}
 		M[e] = 1;

		currElement = G->adjlists[e];
		if (currElement != NULL)
 		{
 			while (currElement != NULL) 		//Parcour of adjs of E.
 			{
 				adj = currElement->value;
				new_cost = cost(G, positions, e, adj) + res_dist[e];

 				if (!want_lighted) 
 				{
					if (G->lit[adj] && M[e] && (res_dist[adj] == -1 || new_cost < res_dist[adj]))
					{
						res_dist[adj] = new_cost;
						res_pred[adj] = e;
						//update(h, adj, res_dist[adj] + heur[adj]);
						heap_update(h, adj, res_dist[adj] + heur[adj]);
					}

 				}
 				else if (M[e] && (res_dist[adj] == -1 || new_cost < res_dist[adj]))
 				{
 					res_dist[adj] = new_cost;
					res_pred[adj] = e;
					//update(h, adj, res_dist[adj] + heur[adj]);
					heap_update(h, adj, res_dist[adj] + heur[adj] * want_lighted);
 				}
 				

				currElement = currElement->next;
 			}
 		}
 	}

 	if (res_dist[end] == -1)
 		errx(1,"Destination not reachable.");

 	free(heur);
 	while (heap_isempty(h))
 		heap_pop(h,&e,&oui);

 	//while (h->next != NULL)
 	//	h_pop(h,&e,&oui);
 	printf("iter - %d\n",iter );
 	free(h);
 	free(M);
}