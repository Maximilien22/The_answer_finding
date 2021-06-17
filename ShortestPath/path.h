#ifndef H_PATH
#define H_PATH

#include "list.h"
double get_distance(double lat1, double lon1, double lat2, double lon2);
void Dijkstra(Graph* G, int start,int end, Couple** positions, double* res_dist, int* res_pred);
void get_data(Couple** positions, int i ,double* x, double* y);
double get_min_way(int start, int end,int* pred,double* dist,struct List* way);
void A_star(Graph* G,Couple** positions, int start, int end, double* res_dist, int* res_pred, unsigned char want_lighted);

#endif