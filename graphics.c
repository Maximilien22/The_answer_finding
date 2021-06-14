#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>

#include <err.h>
#include <math.h>
#include "parsing.h"
#include "tools.h"
#include "path.h"
#include "vector.h"
#include "list.h"

SDL_Surface *screen = NULL;

#define window_name "Why not Finding"
Uint32 white = 0;
Uint32 black = 0;
Uint32 blue = 0;
double zoom = 100000;
double zoom_change = 1.5;
double move_change = 0.0002;
double center_lat = 45.755015 ;
double center_lon = 4.859000;  
double node_size = 2;
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
void draw_line(int pX1, int pY1, int pX2, int pY2, Uint32 color);
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

void draw_line(int pX1, int pY1, int pX2, int pY2, Uint32 color){
	
	SDL_Surface *rectangle1 = SDL_CreateRGBSurface(SDL_HWSURFACE, node_size/1.5, node_size/1.5, 32, 0, 0, 0, 0);
    SDL_FillRect(rectangle1, NULL, color);
		
	SDL_Rect position;
	
	int lenX = pX2 - pX1;
	int lenY = pY2 - pY1;
	float vectorLen = sqrtf(lenX * lenX + lenY * lenY);
	if(vectorLen == 0)
		return;    
	float inclineX = lenX / vectorLen;
    float inclineY = lenY / vectorLen;    
	float x = (int)pX1;    
	float y = (int)pY1;    
	/*int locked = SDL_MUSTLOCK(screen);   
	if(locked)
		SDL_LockSurface(screen);*/
	
	for(int i = 0; i < (int)vectorLen; i+=1){
		
		
		position.x = (int) x;
		position.y = (int) y;
		SDL_BlitSurface(rectangle1, NULL, screen, &position);
	
        x += inclineX;        
		y += inclineY;   
		
		}
		
		/*if(locked)
			SDL_UnlockSurface(screen);*/
		
		
	SDL_FreeSurface(rectangle1);
		
}

void __draw_graph_prof(int x, int* M){
	M[x] = (int) 1;
	
	Couple* cp = &positions[x];
	
	double x1 = 0;
	double y1 = 0;
	
	getScreenCoordonate(cp->y, cp->x, &x1, &y1);
	
	Value_list* adjlists = g->adjlists[x];
	
	if (adjlists!= NULL && adjlists->value>=0){
		if (x == node1Path || x == node2Path){
			print_rectangle(x1, y1, blue, 3);
		}else{
			print_rectangle(x1, y1, black, 1);
		}
		
	}
	
	while (adjlists != NULL){
	
		Couple* cp1 = &(positions[adjlists->value]);
	
		double x2 = 0;
		double y2 = 0;
	
		getScreenCoordonate(cp1->y, cp1->x, &x2, &y2);
			
		draw_line(x1, y1, x2, y2, black);
		
		if (!M[adjlists->value]){
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
		//print_rectangle(x1, y1,blue, 2);
		
		draw_line(x1,y1,x2,y2,blue);
		
		curr = curr->next;
	}
	
	node_size/=3;
}

void draw_graph(){
	SDL_FillRect(screen, NULL, white);
	//SDL_Flip(screen); // updates the screen 
	
	int* M = calloc(g->order, sizeof(int));
	
	for (int i=0; i<g->order; i++){
		
		if (! M[i] ){
			__draw_graph_prof(i, M);
		}
			
	}
		
	draw_path();
	SDL_Flip(screen);
	
}

void check_event(){
    int keep_going = 1;
    SDL_Event event;
		
    while (keep_going)
    {
		
		int x=0;
		int y=0;
		if (SDL_GetMouseState(&x, &y)){
			//print_rectangle(x,y,blue,1);
			//SDL_Flip(screen);
			
			if (node1Path == -1){
				node1Path = getNodeAccordingToPos(x, y);
			}else{
				node2Path = getNodeAccordingToPos(x, y);
			}
			
			draw_graph();
			
		}
		
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                keep_going = 0;
				break;
			//case SDL_VIDEORESIZE:
				//screen = SDL_SetVideoMode(0,0, 32, SDL_FULLSCREEN | SDL_HWSURFACE );
				//draw_graph();
				//break;
						
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
						center_lon += (move_change*200000/zoom);
						draw_graph();
						break;
						
					case SDLK_LEFT:						
						center_lon -= (move_change*200000/zoom);
						draw_graph();
						break;
						
					case SDLK_DOWN:
						center_lat -= (move_change*200000/zoom);
						draw_graph();
						break;
						
					case SDLK_UP:
						center_lat += (move_change*200000/zoom);
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

void print_rectangle(int x, int y, Uint32 color, int size_coef){
    	
	SDL_Surface *rectangle = SDL_CreateRGBSurface(SDL_HWSURFACE, 
	node_size*size_coef, node_size*size_coef, 32, 0, 0, 0, 0);
	
    SDL_FillRect(rectangle, NULL, color);
    SDL_Rect position;
    position.x = x;
    position.y = y;
    SDL_BlitSurface(rectangle, NULL, screen, &position);
	
	SDL_FreeSurface(rectangle);
    
}

void printGraph(GraphInfo * g1, Graph* graph){

	gInfo = g1;
	g = graph;
	
    if (SDL_Init(SDL_INIT_VIDEO) == -1){
		printf("error");
	}
	
	
    screen = SDL_SetVideoMode(0, 0, 32, SDL_FULLSCREEN | SDL_HWSURFACE ); // we do that a first time to get screen width and height
	window_height = screen->h;
	window_width = screen->w;
	
	screen = SDL_SetVideoMode(window_width, window_height, 32, SDL_HWSURFACE );
	
	//printf("w : %i, h:%i\n",window_width,window_height);
				
	if (screen == NULL){
		err(1,"cant create screen, try with a lower resolution");
	}
	
	positions = calloc(g->order, sizeof(Couple));
	
	Couple_list * pos = gInfo->pos;
	
    for (int i=0; i<g->order; i++){
		positions[i] = *(pos->couple);
        pos = pos->next;
    }
	
	//SDL_HWSURFACE: les données seront chargées dans la mémoire vidéo, c'est-à-dire dans la mémoire de votre carte 3D. Avantage : cette mémoire est plus rapide. Défaut : il y a en général moins d'espace dans cette mémoire que dans l'autre (SDL_SWSURFACE).
	
	white = SDL_MapRGB(screen->format, 255, 255, 255);
	black = SDL_MapRGB(screen->format, 0, 0, 0);
	blue = SDL_MapRGB(screen->format, 0, 0, 255);
	
	SDL_WM_SetCaption(window_name, NULL);
	
	draw_graph();
	
    check_event(); 

	
    SDL_Quit(); 
	

// TODO : draw line faster
// TODO : plus rapide quand on zoom

// TODO : memory leaks
// TODO : getShorttestPath elsewhere
}
