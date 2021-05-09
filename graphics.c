#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <err.h>
#include "parsing.h"
#include "tools.h"
#include "path.h"
#include "vector.h"

SDL_Surface *screen = NULL;

#define window_height 480
#define window_width 640
#define node_size 2
#define window_name "Why not Finding"
Uint32 white = 0;
Uint32 black = 0;


void pause()
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
        }
    }
}

void getScreenCoordonate(double lon,double lat, double* x, double* y ){
    double ref_lat1 = 45.753041 ;
    double ref_lon1 = 4.853470;   
    
    /*double ref_lat2 = 45.757043;
    double ref_lon2 = 4.859622;  */


/*

latitude
^                 2
|
|
|
|
|
|
1 _________________> longitude 

*/

   
    *y = window_height- ( lat - ref_lat1 )*100000 ;
    *x =  ( (lon - ref_lon1)*100000 ) ;

	printf("longitude %f becomes %f\n", lon, *x);
	printf("latitude %f becomes %f\n", lat, *y);

}

void print_node(int x, int y){

    
    SDL_Surface *rectangle = SDL_CreateRGBSurface(SDL_HWSURFACE, node_size, node_size, 32, 0, 0, 0, 0);
    SDL_FillRect(rectangle, NULL, black);
    SDL_Rect position;
    position.x = x;
    position.y = y;
    SDL_BlitSurface(rectangle, NULL, screen, &position);

    SDL_Flip(screen);
    SDL_FreeSurface(rectangle);
}


void printGraph(GraphInfo * g){

    SDL_Init(SDL_INIT_VIDEO);
    screen = SDL_SetVideoMode(window_width, window_height, 32, SDL_HWSURFACE); 
	
	if (screen == NULL){
		err(1,"cant create screen, try with a lower resolution");
	}
	//SDL_HWSURFACE: les données seront chargées dans la mémoire vidéo, c'est-à-dire dans la mémoire de votre carte 3D. Avantage : cette mémoire est plus rapide. Défaut : il y a en général moins d'espace dans cette mémoire que dans l'autre (SDL_SWSURFACE).
	
	white = SDL_MapRGB(screen->format, 255, 255, 255);
	black = SDL_MapRGB(screen->format, 0, 0, 0);
	
	SDL_FillRect(screen, NULL, white);
	
	SDL_WM_SetCaption(window_name, NULL);
	SDL_Flip(screen); /* updates the screen */

    

	

    Couple_list * pos = g->pos;
	
    while (pos != NULL){
        double lat = pos->couple->x;
        double lon= pos->couple->y;
		
		double x = 0;
        double y = 0;

        getScreenCoordonate( lon,  lat, &x, &y);

		print_node(x, y);

        pos = pos->next;
    }
    

    pause(); 

   
    SDL_Quit(); 

//TODO : faire que ca s'adapte a la taille de la fenetre, pas juste * 10000
// TODO : dessiner les arcs
}
