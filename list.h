#ifndef H_LIST
#define H_LIST

#include "list.h"

typedef struct List List;
struct List
{
    int val;
    int cost;
    List *next;
};

List *initlist();

void append(struct List *list, int nv);

void update(struct List* list, int val, int cost);

void h_pop(struct List* list, int* val, int* cost);

int pop(struct List *liste);

void afficherListe(struct List *liste);

void delel(struct List *liste, int t);

int lvalue(struct List *liste, int t);

void lchangeval(struct List *liste, int rech, int newval);

void free_List(struct List *list);

#endif