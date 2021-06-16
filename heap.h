#ifndef H_HEAP
#define H_HEAP

#include "heap.h"

typedef struct HEAP heap;
struct heap
{
    int val;
    double cost;
    struct heap *left;
    struct heap *right;
    int nb;
};

struct heap *initheap();

int heap_isempty(struct heap* h);

void heap_update(struct heap* h, int val, double cost);

void heap_pop(struct heap* h, int* val, double* cost);

void heap_print(struct heap* h, int t);

#endif