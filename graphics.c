#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <err.h>
#include <math.h>
#include "parsing.h"
#include "tools.h"
#include "path.h"
#include "vector.h"

SDL_Surface *screen = NULL;

#define window_height 480
#define window_width 640
#define window_name "Why not Finding"
Uint32 white = 0;
Uint32 black = 0;
double zoom = 100000;
double zoom_change = 1.5;
double move_change = 0.0002;
double center_lat = 45.755015 ;
double center_lon = 4.859000;  
int node_size = 2;
GraphInfo * gInfo = NULL; 
Graph * g = NULL;
SDL_Surface *rectangle = NULL;


void printGraph(GraphInfo * g1, Graph* g);
void draw_graph();
void check_event();
void getScreenCoordonate(double lon,double lat, double* x, double* y );
void print_node(int x, int y);
void draw_line(int pX1, int pY1, int pX2, int pY2);


void draw_line(int pX1, int pY1, int pX2, int pY2){
	int lenX = pX2 - pX1;
	int lenY = pY2 - pY1;
	float vectorLen = sqrtf(lenX * lenX + lenY * lenY);
	if(vectorLen == 0)
		return;    
	float inclineX = lenX / vectorLen;
    float inclineY = lenY / vectorLen;    
	float x = (int)pX1;    
	float y = (int)pY1;    
	int locked = SDL_MUSTLOCK(screen);   
	if(locked)        SDL_LockSurface(screen);
	for(int i = 0; i < (int)vectorLen; ++i){
		
		SDL_Surface *rectangle1 = SDL_CreateRGBSurface(SDL_HWSURFACE, 1, 1, 32, 0, 0, 0, 0);
        SDL_FillRect(rectangle1, NULL, black);
		SDL_Rect position;
		position.x = (int) x;
		position.y = (int) y;
		SDL_BlitSurface(rectangle1, NULL, screen, &position);
		
		SDL_FreeSurface(rectangle1);
	
		//SDL_PutPixel(screen, (int)x, (int)y, black);
	
        x += inclineX;        
		y += inclineY;    }    
		if(locked)
			SDL_UnlockSurface(screen);
	}

void draw_graph(){
	SDL_FillRect(screen, NULL, white);
	SDL_Flip(screen); /* updates the screen */
	
	//int* marked = calloc(g->order, sizeof(int));
	
	Couple_list * pos = gInfo->pos;
	
    while (pos != NULL){
        double lat = pos->couple->x;
        double lon= pos->couple->y;
		
		double x = 0;
        double y = 0;

        getScreenCoordonate( lon,  lat, &x, &y);

		print_node(x, y);

        pos = pos->next;
    }
	SDL_Flip(screen);
	
}

void check_event()
{
    int keep_going = 1;
    SDL_Event event;
	
    while (keep_going)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                keep_going = 0;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
					case SDLK_KP_PLUS :
						zoom*=zoom_change;
						draw_graph();	
						break;
						
					case SDLK_KP_MINUS:
						zoom/=zoom_change;
						draw_graph();
						break;
						
					case SDLK_RIGHT:
						center_lon += move_change;
						draw_graph();
						break;
						
					case SDLK_LEFT:						
						center_lon -= move_change;
						draw_graph();
						break;
						
					case SDLK_DOWN:
						center_lat -= move_change;
						draw_graph();
						break;
						
					case SDLK_UP:
						center_lat += move_change;
						draw_graph();
						break;
					default:
						break;
				}
				break;
			
        }
    }
}

void getScreenCoordonate(double lon,double lat, double* x, double* y ){
    
/*

latitude
^                 
|
|
|		  1
|
|
|
 _________________> longitude 

*/

   
    *y = (window_height/2 - ( lat - center_lat )*zoom)  ; // =  (window_height- ( lat - center_lat )*zoom) - (window_height/2)
    *x =  ( (lon - center_lon)*zoom ) + (window_width/2) ;

	//printf("longitude %f becomes %f\n", lon, *x);
	//printf("latitude %f becomes %f\n", lat, *y);

}


void print_node(int x, int y){
    	
    SDL_FillRect(rectangle, NULL, black);
    SDL_Rect position;
    position.x = x;
    position.y = y;
    SDL_BlitSurface(rectangle, NULL, screen, &position);
    
}


void printGraph(GraphInfo * g1, Graph* graph){

	gInfo = g1;
	g = graph;
	
    SDL_Init(SDL_INIT_VIDEO);
    screen = SDL_SetVideoMode(window_width, window_height, 32, SDL_HWSURFACE); 
	
	if (screen == NULL){
		err(1,"cant create screen, try with a lower resolution");
	}
	//SDL_HWSURFACE: les données seront chargées dans la mémoire vidéo, c'est-à-dire dans la mémoire de votre carte 3D. Avantage : cette mémoire est plus rapide. Défaut : il y a en général moins d'espace dans cette mémoire que dans l'autre (SDL_SWSURFACE).
	
	white = SDL_MapRGB(screen->format, 255, 255, 255);
	black = SDL_MapRGB(screen->format, 0, 0, 0);
	
	SDL_WM_SetCaption(window_name, NULL);
	rectangle = SDL_CreateRGBSurface(SDL_HWSURFACE, node_size, node_size, 32, 0, 0, 0, 0);
    
	draw_graph();
    
	
    check_event(); 

	SDL_FreeSurface(rectangle);
    SDL_Quit(); 

// TODO : dessiner les arcs, parcours profondeur 
// TODO : faire augmenter diminuer taille node quand zoom dezoom

}
