#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vector.h"

vector *vector_new()
{
    vector* vect = calloc(1, sizeof(vector));

    if (vect == NULL){
    	//errx("not enough memory to allocate a new vector");
    	printf("not enough memory to allocate a new vector");
    }

    vect-> max_size = 100;
    vect-> size = 0;
    vect-> data = calloc(vect->max_size, sizeof(unsigned long));

    return vect;
}

void vector_free(vector *v)
{
    free(v->data);
    free(v);
}

void double_max_size(vector *v)
{
	v->max_size = (v->max_size*1.2) + 5;
    v->data = realloc(v->data, sizeof(unsigned long) * (v->max_size));

}

void vector_reset(vector *v){
	memset(v->data,0,v->max_size);
	v->size = 0;
}

void vector_add(vector *v, unsigned long x)
{
    if (v->max_size <= v->size)
    	double_max_size(v);

    v->data[v->size] = x;

    v->size+=1;

}


void vector_print(vector *v){

printf("---------PRINTING VECTOR------------\n\nmax_size : %li\n",v->max_size );
printf("size : %li\n",v->size );

	for (size_t i=0; i<v->max_size; i++){
		printf("data[%li]=%lu\n",i,v->data[i]);
	}
	printf("\n\n");
}
/*
int main (){


vector *vect = vector_new();

vector_push(vect, 5);
vector_push(vect, 9);
vector_push(vect, 7);
vector_push(vect, 3);
vector_push(vect, 5);
vector_push(vect, 5);
vector_push(vect, 5);
vector_print(vect);




int* x = malloc(sizeof(int));
vector_remove(vect, 3, x);
vector_print(vect);


vector_remove(vect, 0, x);
vector_print(vect);


vector_remove(vect, vect->size, x);
vector_print(vect);




vector_insert(vect, vect->max_size, 555);
vector_print(vect);


vector_insert(vect, vect->size, 444);
vector_print(vect);



vector_insert(vect, 1000, 888);
vector_print(vect);


vector_insert(vect, 0, 333);
vector_print(vect);


vector_insert(vect, 4, 111);
vector_print(vect);


vector_free(vect);

}*/