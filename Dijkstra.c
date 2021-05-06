#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
// #include <unistd.h>
// #include <err.h>
#include "parsing.h"
#include "tools.h"
#include "list.h"

double get_distance(double lat1, double lon1, double lat2, double lon2)
{
	double dist;
  	if ((lat1 == lat2) && (lon1 == lon2))
    	return 0;

    dist = sin(deg2rad(lat1)) * sin(deg2rad(lat2)) + cos(deg2rad(lat1)) * cos(deg2rad(lat2)) * cos(deg2rad(lon1 - lon2));
   	dist = acos(dist);
   	dist = rad2deg(dist);
   	dist = dist * 60 * 1.1515;
    return 100 * dist * 1.609344;
}

int cost(Graph* G, GraphInfo* gInfo, int s1, int s2)
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

			return (int) get_distance(cpl1->couple->x, cpl1->couple->y, 
				cpl2->couple->x, cpl2->couple->y);
		}
		currElement = currElement->next;
	}

	return -1;
}

int min_of_list(struct List *M, struct List *dist)
{
    int res = -1;
    struct List *t = M;
    int i = 0;
    while (t != NULL)
    {
        i = t->val;
        if (res == -1 || lvalue(dist, i) < lvalue(dist, res) )
        {
            if (lvalue(dist, i) != -1)
            {
                res = i;
            }

        }
        i += 1;
        t = t->next;
    }
    return res;
}


struct List* Dijkstra(Graph* G,  GraphInfo* gInfo, int start)
{
    //int order = G->order;
    struct List *M = malloc(sizeof(List));
    struct List *dist = malloc(sizeof(List));
    struct List *pred = malloc(sizeof(List));


    for(int i = 0; i < G->order; i++)
    {
        append(M, i);
        append(M, start);
        append(M, cost(G, gInfo, start, i));
    }
    
    delel(M, start);

    while (M != NULL)
    {
        int t = min_of_list(M, dist);
        if(t == -1)
        {
            free(M);
            free(dist);
            return pred;
        }

        delel(M, t);

        // int i = 0;
        Value_list* adj = G->adjlists[t];
        while (adj != NULL)
        {
            int value = adj->value;
            if (lvalue(dist, value) == -1 || lvalue(dist, t) + cost(G, gInfo, t, value) < lvalue(dist, value))
            {
                lchangeval(dist, value, lvalue(dist, t) + cost(G, gInfo, t, value));
                lchangeval(pred, value, t);
            }
            adj = adj->next;
        }
        
    }

    free(M);
    free(dist);
    return pred;
}