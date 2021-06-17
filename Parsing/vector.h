#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <stdlib.h>

typedef struct vector vector;
struct vector
{
    size_t max_size; // the numbers of bytes allocated
    size_t size;// the numbers if bytes used
    unsigned long *data;// the array
};

void vector_reset(vector *v);

vector *vector_new();

void vector_free(vector *v);

void double_capacity(vector *v);

void vector_add(vector *v, unsigned long x);

void vector_print(vector *v);


#endif