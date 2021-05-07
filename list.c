#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <unistd.h>


typedef struct List List;
struct List
{
    int val;
    List *next;
};

List *initlist()
{
    List *list = malloc(sizeof(*list));
    if (list)
    {
        list->next = NULL;
    }
    return list;
}

void append(struct List *list, int nv)
{
    List *nouveau = malloc(sizeof(*nouveau));
    if (nouveau)
    {
        nouveau->val = nv;
    }
    nouveau->next = list->next;
    list->next = nouveau;
    return;
}

int pop(struct List *liste)
{
    if (liste->next == NULL)
    {
        printf("liste vide; pop impossible" );
    }

    int val = liste->next->val;
    List *t = liste->next;
    liste->next = t->next;
    free(t);
    return val;
}

void afficherListe(struct List *liste)
{
    if (liste->next == NULL)
    {
        printf("liste vide\n");
        return;
    }
    List *t = liste->next;
    while (t != NULL)
    {
        printf("%d", t->val);
        if (t->next != NULL)
        {
        	printf(" -> ");
        }
        t = t->next;
        
    }
    printf("\n");
    return;
}

void delel(struct List *liste, int rech)
{
    List *t = liste->next;
    while(t != NULL)
    {
        if (t->val == rech)
        {
            t = t->next;
            return;
        }
    }
    return;
}

int lvalue(struct List *liste, int rech)
{
    if (rech == -1)
    {
        return -1;
    }

    List *t = liste->next;
    for (int i = 0; i < rech; i++)
    {
        t = t->next;
    }
    return t->val;
}

void lchangeval(struct List *liste, int rech, int newval)
{
    List *t = liste->next;
    for (int i = 0; i < rech; i++)
    {
        t = t->next;
    }
    t->val = newval;
    return;
}