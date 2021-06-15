#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>

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
Uint8 path_color [4] = {0,0,255, 255};
Uint8 not_way_color [4] = {200,200,200, 255};
Uint8 litColor [4] = {255,215,0, 150};
Uint8 notlitColor [4] = {50,50,50, 255};

DoubleInt* initialPositions = NULL;



const double zoom_change = 1.5;
const double move_change = 50;
const double center_lat = 45.755015 ;
const double center_lon = 4.859000;  

double zoom = 100000;
double node_size = 2;
int shift_x = 0;
int shift_y = 0;

GraphInfo * gInfo = NULL; 
Graph * g = NULL;
Couple* positions = NULL;
struct List* path = NULL;
int window_height = 480;
int window_width  = 640;
int node1Path = -1;
int node2Path = -1;


void draw_graph();
void check_event();
void getScreenCoordonate(double lon,double lat, double* x, double* y );
void print_rectangle(int x, int y, Uint32 color, int sizeCoef);
void draw_line(int pX1, int pY1, int pX2, int pY2, Uint8 * color);
void draw_path();
void updateNodeSize();
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

	
	Dijkstra(g,start,gInfo,dist,pred);


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
			// on rentre jamais la ?
			closest = cp;
			node = i;
		}
	
	}

	return node;
}

void updateNodeSize(){
	if (zoom >= 90000 && zoom < 510000 ){
		node_size = (double) zoom / 50000; // therefore if zoom = 10000 it returs 2 ( the initial stuff)
		//printf("%f\n",node_size);
	}
}

void draw_line(int pX1, int pY1, int pX2, int pY2, Uint8 *color){
		
	SDL_SetRenderDrawColor(renderer, color[0], color[1], color[2], color[3]);
	
	if (abs(pX1-pX2) <50){
		SDL_RenderDrawLine(renderer, pX1+1 , pY1 , pX2+1 , pY2);
		SDL_RenderDrawLine(renderer, pX1 , pY1 , pX2 , pY2);
		SDL_RenderDrawLine(renderer, pX1-1 , pY1 , pX2-1 , pY2);
		
	}else{ 	
		SDL_RenderDrawLine(renderer, pX1 , pY1+1 , pX2 , pY2+1);
		SDL_RenderDrawLine(renderer, pX1 , pY1 , pX2 , pY2);
		SDL_RenderDrawLine(renderer, pX1 , pY1-1 , pX2 , pY2-1);
	}
}

void __draw_graph_prof(int x, int* M){
	M[x] = (int) 1;
	
	
	double x1 = initialPositions[x].x + shift_x ;
	double y1 = initialPositions[x].y + shift_y;
	
	Value_list* adjlists = g->adjlists[x];
	
	/*SDL_SetRenderDrawColor(renderer, black[0], black[1], black[2], black[3]);
	SDL_Rect rect = {x1,y1,node_size,node_size};
	SDL_RenderDrawRect(renderer, &rect);
	*/
			
	if (adjlists!= NULL && adjlists->value >= 0 && (x == node1Path || x == node2Path) ){
			
			SDL_SetRenderDrawColor(renderer, path_color[0], path_color[1], path_color[2], path_color[3]);
			SDL_Rect rect = {x1,y1,node_size*4,node_size*4};
			SDL_RenderDrawRect(renderer, &rect);
	}
	
	while (adjlists != NULL){
			
		if (!M[adjlists->value]){
			
			double x2 = initialPositions[adjlists->value].x + shift_x ;
			double y2 = initialPositions[adjlists->value].y + shift_y;
		
		
		
			if (g->lit[x] && g->lit[adjlists->value])
				draw_line(x1, y1, x2, y2, litColor);
			else if (g->notLit[x] || !g->notLit[adjlists->value])
				draw_line(x1, y1, x2, y2, notlitColor);
			else
				draw_line(x1, y1, x2, y2, not_way_color);
			
			__draw_graph_prof(adjlists->value, M);
		}
		adjlists = adjlists->next;
	}
}

void draw_path(){
	
	if (path == NULL)
		return;
	
	node_size*=3;
	
	struct List* curr = path->next;
	while (curr != NULL && curr->next!= NULL){
		int node1 = curr->val;
		int node2 = curr->next->val;
		
		Couple* cp1 = &positions[node1];
		Couple* cp2 = &positions[node2];
		
		double x1 = 0;
		double y1 = 0;
		
		double x2 = 0;
		double y2 = 0;
		
		//printf("je recup le noeud : %i, lat = %f, lon =%f\n", node, cp->y, cp->x);
		
		getScreenCoordonate(cp1->y, cp1->x, &x1, &y1);
		getScreenCoordonate(cp2->y, cp2->x, &x2, &y2);
	
		//printf("y : %f, x : %f, x1 : %f, y1 : %f\n", (cp->y), (cp->x), x1, y1);
		//print_rectangle(x1, y1,path_color, 2);
		
		draw_line(x1,y1,x2,y2,path_color);
		
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
		
		if (! M[i] ){
			__draw_graph_prof(i, M);
		}
			
	}
		
	draw_path();
	
	//SDL_SetRenderDrawColor(renderer, black[0], black[1], black[2], black[3]);
	//SDL_RenderDrawRects(renderer,rects, g->order);
	SDL_RenderPresent(renderer);
	free(M);
	//free(rects);
	
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
			}else{
				node2Path = getNodeAccordingToPos(x, y);
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
					
							window_width = event.window.data1;
							window_height = event.window.data2;
							draw_graph();
					}
				break;
						
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
					case SDLK_KP_PLUS :
						if (zoom<= 500000){
							zoom*=zoom_change;
							updateNodeSize();
							draw_graph();
						}
						break;						
						
					case SDLK_KP_MINUS:						
						if (zoom >= 10000){
							zoom/=zoom_change;
							updateNodeSize();
							draw_graph();
						}
						break;
						
					case SDLK_RIGHT:
						shift_x -= move_change;
						draw_graph();
						break;
						
					case SDLK_LEFT:						
						shift_x += move_change;
						draw_graph();
						break;
						
					case SDLK_DOWN:
						shift_y -= move_change;
						draw_graph();
						break;
						
					case SDLK_UP:
						shift_y += move_change;
						draw_graph();
						break;
						
					case SDLK_ESCAPE:
						node1Path = -1;
						node2Path = -1;
						path = NULL;
						draw_graph();
						break;
					case SDLK_RETURN:
						path = getShortestPath(node1Path, node2Path);
						draw_graph();
						
					default:
						break;
				}
				break;
			
        }
    }
}

void getScreenCoordonate(double lon,double lat, double* x, double* y ){
    


//	latitude
//	^                 
//	|
//	|
//	|		  1
//	|
//	|
//	|
//	 _________________> longitude 



   
    *y = (window_height/2 - ( lat - center_lat )*zoom)  ; // =  (window_height- ( lat - center_lat )*zoom) - (window_height/2)
    *x =  ( (lon - center_lon)*zoom ) + (window_width/2) ;

	//printf("longitude %f becomes %f\n", lon, *x);
	//printf("latitude %f becomes %f\n", lat, *y);

}

void calculate_initialPos(){
	
	for (int node=0; node<g->order; node++){
		
		Couple* cp = &positions[node];
	
		double x1 = 0;
		double y1 = 0;
	
		
		getScreenCoordonate(cp->y, cp->x, &x1, &y1);
	
		initialPositions[node].x = x1;
		initialPositions[node].y = y1;
		
	}
}

void displayMap(GraphInfo * g1, Graph* graph){

	gInfo = g1;
	g = graph;
	
	
	
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
	
	initialPositions = calloc((g->order), sizeof(DoubleInt));
	
	calculate_initialPos();
	draw_graph();
	
    check_event(); 

	free(initialPositions);
	free(positions);
	if (path!= NULL)
		free_List(path);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
    SDL_Quit(); 
	

// TODO : draw line faster
// TODO : plus rapide quand on zoom

// TODO : memory leaks
// TODO : getShorttestPath elsewhere
}
