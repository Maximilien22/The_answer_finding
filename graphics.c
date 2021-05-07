#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include "parsing.h"
#include "tools.h"
#include "path.h"
#include "vector.h"
 


SDL_Surface *ecran = NULL;

int window_height = 480;
int window_width = 640;

void pause()
{
    int continuer = 1;
    SDL_Event event;
 
    while (continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                continuer = 0;
        }
    }
}

void getScreenCoordonate(double lon,double lat, double* x, double* y ){
    double ref_lat1 = 4.853470;
    double ref_lon1 = 45.753041;   
    
    /*double ref_lat2 = 45.757043;
    double ref_lon2 = 4.859622;  */


/*

longitude
^                 2
|
|
|
|
|
|
1 _________________> latitude 

*/
   
    *y =window_height- ( (lon - ref_lon1) * 20000 ) ;
    *x =( lat - ref_lat1 ) *20000;



}

void print_sommet(int x, int y){

    
    SDL_Surface *rectangle = SDL_CreateRGBSurface(SDL_HWSURFACE, 5, 5, 32, 0, 0, 0, 0);
    SDL_FillRect(rectangle, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
    SDL_Rect position;
    position.x = x;
    position.y = y;
    SDL_BlitSurface(rectangle, NULL, ecran, &position);

    SDL_Flip(ecran);
     //SDL_FreeSurface(rectangle);
}


void printGraph(GraphInfo * g){



    SDL_Init(SDL_INIT_VIDEO);
 
    SDL_SetVideoMode(window_width, window_height, 32, SDL_HWSURFACE); 
    
    ecran = SDL_SetVideoMode(window_width, window_height, 32, SDL_HWSURFACE);
 
    //double lat_p1 = 4.855562;
    //double lon_p1 = 45.754780;



    /*double* lat_p2;
    double* lon_p2;*/

    


    Couple_list * pos = g->pos;

    while (pos != NULL){
        double x = pos->couple->x;
        double y= pos->couple->y;

        getScreenCoordonate( y,  x, &y, &x );

        /*printf("x : %f\n", y);
        printf("y : %f\n", x);*/


        pos = pos->next;
    }
    

    pause(); 

   
    SDL_Quit(); 

}
