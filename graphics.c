#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
//#import "lib/SDL2.h"
#include <err.h>
#include <math.h>
#include "parsing.h"
#include "tools.h"
#include "path.h"
#include "vector.h"
#include "list.h"

#define window_name "Why not Finding"
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

Uint8 background [4] = {0,0,0, 0};
Uint8 path_color [4] = {255,255,255, 255};
Uint8 not_way_color [4] = {100,100,100, 255};
Uint8 litColor [4] = {255,215,0, 150};
Uint8 notlitColor [4] = {100,100,100, 255};

DoubleInt* screenPosition = NULL;



//const double zoom_change = 1.5;
double center_lat = 45.755015 ;
double center_lon = 4.859000;  

double zoom = 100000;
double node_size = 2;
double move_change = 40;
double zoom_change = 1.5;

GraphInfo * gInfo = NULL; 
Graph * g = NULL;
Couple* positions = NULL;
struct List* path = NULL;
const int window_height = 480;
const int window_width  = 640;
int current_height = 480;
int current_width = 640;
int node1Path = -1;
int node2Path = -1;


void draw_graph();
void check_event();
void getScreenCoordonate(double lon,double lat, double* x, double* y );
void print_rectangle(int x, int y, Uint32 color, int sizeCoef);
void draw_line(int pX1, int pY1, int pX2, int pY2, Uint8 * color, int isBig);
void draw_path();
int getNodeAccordingToPos(int x, int y);
double toAbs (double x);
struct List* getShortestPath(int start, int end );

struct List* getShortestPath(int start, int end ){
	
	if (start == -1 || end ==-1 || g==NULL || gInfo ==NULL){
		printf("error");
		return NULL;
	}
	double* dist = calloc(g->order,sizeof(double));
	int* pred = calloc( g->order,sizeof(int));

	
	//Dijkstra(g,start,gInfo,dist,pred);
	A_star(g,gInfo, start, end, dist, pred, 1);



	struct List* way = initlist();
	Couple_list* cpl = gInfo->pos;

	int i=0;
	while (cpl != NULL){
		if (i==start || i == end){
			printf("idx : %i, lat, lon : %f, %f\n",i, cpl->couple->x,cpl->couple->y);
		}
		cpl=cpl->next;
		i++;
	}

	double distance = get_min_way(start,end,pred,dist,way);

	printf("Distance entre le sommet %d et %d= %f\n",start,end,distance );
	
	free(dist);
	free(pred);
	
	return way;
	
}

double toAbs (double x){
	if (x<0){
		return -1*x;
	}
	return x;
}

int getNodeAccordingToPos(int x,int y){
	double lat;
	double lon;
	
	
    lat = (-y + window_height/2 ) /zoom + center_lat ;
	//printf("lat : %f  ",lat);
	
    lon = (x - (window_width/2) ) /zoom + center_lon ;
	//printf("lon  %f\n",lon);
	
	Couple * closest = &positions[0];
	int node = 0;
	
	
	for (int i=0; i<g->order; i++){
	
		Couple * cp = &positions[i];
		
		/*double j =toAbs(lat - closest->x) + toAbs(lon-closest->y);
		printf ("1 lat:%f, lat_closest:%f, lon:%f, lon_closest:%f, diff1:%f\n",lat,closest->x, lon, closest->y, j);
		
		double j2 =toAbs(lat - cp->x) + toAbs(lon - cp->y);
		printf ("2 lat:%f, lat_curr:%f, lon:%f, lon_curr:%f, diff2:%f\n\n",lat, cp->x,lon,cp->y,j2
		 );*/
		
		if ( (toAbs(lat - closest->x) + toAbs(lon - closest->y)) > (toAbs(lat - cp->x) + toAbs(lon - cp->y))	){
			if ((g->lit[i]) && g->nodeNames[i]!=NULL  ){
				closest = cp;
				node = i;
			}
		}
	
	}

	return node;
}

void draw_line(int pX1, int pY1, int pX2, int pY2, Uint8 *color, int isBig){
			
	if (isBig){
		if (abs(pX1-pX2)>abs(pY1-pY2)){
			SDL_RenderDrawLine(renderer, pX1 , pY1+1 , pX2 , pY2+1);
			SDL_RenderDrawLine(renderer, pX1 , pY1 , pX2 , pY2);
			SDL_RenderDrawLine(renderer, pX1 , pY1-1 , pX2 , pY2-1);
		}else{ 
			SDL_RenderDrawLine(renderer, pX1+1 , pY1 , pX2+1 , pY2);
			SDL_RenderDrawLine(renderer, pX1 , pY1 , pX2 , pY2);
			SDL_RenderDrawLine(renderer, pX1-1 , pY1 , pX2-1 , pY2);
		}
	}else{
		SDL_SetRenderDrawColor(renderer, color[0], color[1], color[2], color[3]);
	}
	
	SDL_RenderDrawLine(renderer, pX1 , pY1 , pX2 , pY2);
}

void draw_path(){
	
	if (path == NULL)
		return;
	
	node_size*=3;
	
	struct List* curr = path->next;
	while (curr != NULL && curr->next!= NULL){
		int node1 = curr->val;
		int node2 = curr->next->val;
		
		double x1 = screenPosition[node1].x ;
		double y1 = screenPosition[node1].y ;
		
		double x2 = screenPosition[node2].x ;
		double y2 = screenPosition[node2].y ;
		
	
		//printf("y : %f, x : %f, x1 : %f, y1 : %f\n", (cp->y), (cp->x), x1, y1);
		//print_rectangle(x1, y1,path_color, 2);
		
		draw_line(x1,y1,x2,y2,path_color, 1);
		
		curr = curr->next;
	}
	
	node_size/=3;
}

void draw_graph(){
	SDL_SetRenderDrawColor(renderer, background[0], background[1], background[2], background[3]);
	SDL_RenderClear(renderer);
	
	//SDL_Rect * rects = calloc(g->order, sizeof(SDL_Rect));
	
	int* M = calloc(g->order, sizeof(int));
	
	for (int i=0; i<g->order; i++){
		
		M[i]= (int) 1;
		
		Value_list* adjlists = g->adjlists[i];
	
		while (adjlists != NULL){
		
				if (!M[adjlists->value]){
					
					double x1 = screenPosition[i].x;
					double y1 = screenPosition[i].y;
		
					double x2 = screenPosition[adjlists->value].x ;
					double y2 = screenPosition[adjlists->value].y ;
				
				
					if (g->lit[i] && g->lit[adjlists->value] && g->nodeNames[i]!=NULL && g->nodeNames[adjlists->value]!=NULL)
						draw_line(x1, y1, x2, y2, litColor,0);
					else if (g->notLit[i] && g->notLit[adjlists->value])
						draw_line(x1, y1, x2, y2, notlitColor,0);
					else
						draw_line(x1, y1, x2, y2, not_way_color,0);
					
				}
				
			adjlists = adjlists->next;
		}
		
			
	}
	
	SDL_SetRenderDrawColor(renderer, path_color[0], path_color[1], path_color[2], path_color[3]);
	
	if (node1Path != -1){
		double x1 = screenPosition[node1Path].x ;
		double y1 = screenPosition[node1Path].y ;
		
		SDL_Rect rect = {x1,y1,node_size*4,node_size*4};
		SDL_RenderFillRect(renderer, &rect);
	}	
	if (node2Path != -1){
		double x2 = screenPosition[node2Path].x  ;
		double y2 = screenPosition[node2Path].y ;
		
		SDL_Rect rect = {x2,y2,node_size*4,node_size*4};
		SDL_RenderFillRect(renderer, &rect);
	}	
	
	
			
	draw_path();
	
	//SDL_SetRenderDrawColor(renderer, black[0], black[1], black[2], black[3]);
	//SDL_RenderDrawRects(renderer,rects, g->order);
	SDL_RenderPresent(renderer);
	free(M);
	//free(rects);
	
}

void recalculateScreenPositions(){
	
	
		
	for(int i = 0; i<g->order ; i++){
		
		Couple* cp = &positions[i];
	
		double lon = cp->y;
		double lat = cp->x;
	
	
		screenPosition[i].x = ( (lon - center_lon)*zoom ) + (window_width/2) ;
		screenPosition[i].y = (window_height/2 - ( lat - center_lat )*zoom);
	}
}

void check_event(){
    int keep_going = 1;
    SDL_Event event;
		
    while (keep_going)
    {
		
		int x=0;
		int y=0;
		if (SDL_GetMouseState(&x, &y)){
			//print_rectangle(x,y,path_color,1);
			//SDL_Flip(screen);
			
			if (node1Path == -1){
				node1Path = getNodeAccordingToPos(x, y);
			}else if (node2Path ==-1) {
				node2Path = getNodeAccordingToPos(x, y);
				
			}else{ // node1Path != -1 && node2PathPath != -1
				node2Path = getNodeAccordingToPos(x, y);
			}
			
			
		if (node1Path!=-1 && node2Path!=-1){
			printf("node 1 : %s\n", g->nodeNames[node1Path]);
			printf("node 2 : %s\n\n", g->nodeNames[node2Path]);
		}
	
			draw_graph(); //TODO dessiner que les deux points
			
		}
		
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                keep_going = 0;
				break;
			//case SDL_VIDEORESIZE:
				//screen = SDL_SetVideoMode(0,0, 32, SDL_FULLSCREEN | SDL_SWSURFACE );
				//draw_graph();
				//break;
			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED){
					
							current_width = event.window.data1;
							current_height = event.window.data2;
							draw_graph();
					}
				break;
						
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
					case SDLK_KP_PLUS :
						zoom*=zoom_change;
						recalculateScreenPositions();
						draw_graph();
						break;						
						
					case SDLK_KP_MINUS:		
					
						zoom/=zoom_change;
						recalculateScreenPositions();
						draw_graph();					
						break;
						
					case SDLK_RIGHT:
					
						center_lon += (move_change/zoom);
						recalculateScreenPositions();
						draw_graph();
						break;
						
					case SDLK_LEFT:						
						
						center_lon -= (move_change/zoom);
						recalculateScreenPositions();
						draw_graph();
						break;
						
					case SDLK_DOWN:
						
						center_lat -= (move_change/zoom);
						recalculateScreenPositions();
						draw_graph();
						break;
						
					case SDLK_UP:
						
						center_lat += (move_change/zoom);
						recalculateScreenPositions();
						draw_graph();
						break;
						
					case SDLK_ESCAPE:
						node1Path = -1;
						node2Path = -1;
						if (path!=NULL)
							free_List(path);
						path = NULL;
						draw_graph();
						break;
					case SDLK_RETURN:
						if (path!=NULL)
							free_List(path);
						path = getShortestPath(node1Path, node2Path);
						draw_graph();
						
					default:
						break;
				}
				break;
			
        }
    }
}

void displayMap(GraphInfo * g1, Graph* graph){

	gInfo = g1;
	g = graph;

	
	/*for (int i=0; i<g->order; i++){
		 printf("\n%s",g->nodeNames[i]);
	}*/
	
	
    if (SDL_Init(SDL_INIT_VIDEO) == -1){
		printf("Error while initialising sdl : %s",SDL_GetError());
		return ;
	}
	
	window = SDL_CreateWindow(window_name,
                              0,
                              0,
                              window_width,
                              window_height,SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
							  
	if(window == NULL)
	{
		printf("Error while creating window: %s",SDL_GetError());
		return ;
	}

	//SDL_GetWindowSize(window, &window_width, &window_height);
	
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	
	if(renderer == NULL)
	{
		printf("error while creating renderer : %s",SDL_GetError());
		return ;
	}
	
	
	positions = calloc(g->order, sizeof(Couple));
	
	Couple_list * pos = gInfo->pos;
	
    for (int i=0; i<g->order; i++){
		positions[i] = *(pos->couple);
        pos = pos->next;
    }
	
	screenPosition = calloc((g->order), sizeof(DoubleInt));
	
	recalculateScreenPositions();
	draw_graph();
	
    check_event(); 

	free(screenPosition);
	free(positions);
	
	if (path!= NULL)
		free_List(path);
	
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
    SDL_Quit(); 
	

}
