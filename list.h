#ifndef H_LIST
#define H_LIST

#include "list.h"

typedef struct List List;
struct List
{
    void val;
    List *next;
};

List *initlist();

void append(List *list, int nv);

int pop(List *liste);

void afficherListe(Liste *liste);

void delel(List *liste, int t);

int lvalue(List *liste, int t);

void lchangeval(List *liste, int rech, int newval);

#endif