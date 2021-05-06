#include "parsing.h"
#include "tools.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <unistd.h>
// #include <err.h>


struct List* Dijkstra(Graph* G, int start)
{
    int order = G->order;
    struct List *M = malloc(sizeof(List));
    struct List *dist = malloc(sizeof(List));
    struct List *pred = malloc(sizeof(List));
    for(int i = 0; i < G->order; i++)
    {
        append(M, i);
        append(M, start);
        append(M, cost(G, start, i));
    }
    
    delel(M, start);

    while (M != NULL)
    {
        int t = min_of_list(M, dist);
        if(t == -1)
        {
            return pred;
        }

        delel(M, t);

        

        for (int i = 0; i < G->order; i++)
        {
            int t2 = G->adjlists[t];
            if ()
            {
                /* code */
            }
        }
        
    }
    return pred;
}