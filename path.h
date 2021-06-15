#ifndef H_PATH
#define H_PATH

#include "list.h"
double get_distance(double lat1, double lon1, double lat2, double lon2);
void Dijkstra(Graph* G, int start, GraphInfo* gInfo, double* res_dist, int* res_pred);
//Get x = longitude and y = latitude of a edge.
void get_data(GraphInfo* gInfo, int i ,double* x, double* y);
double get_min_way(int start, int end,int* pred,double* dist,struct List* way);
void A_start(Graph* G,GraphInfo* gInfo, int start, int end, double* res_dist, int* res_pred);

#endif