#ifndef H_PATH
#define H_PATH

double get_distance(double lat1, double lon1, double lat2, double lon2);
void Dijkstra(Graph* G, int start, GraphInfo* gInfo, double* res_dist, int* res_pred);

#endif