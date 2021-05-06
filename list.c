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
        list->val = NULL;
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
    nouveau->next = list;
    list = nouveau;
    return;
}

int pop(struct List *liste)
{
    if (liste == NULL)
    {
        exit(EXIT_FAILURE);
    }

    int val = liste->val;
    List *t = liste;
    liste = liste->next;
    free(t);
    return val;
}

void afficherListe(struct List *liste)
{
    if (liste == NULL)
    {
        printf("liste vide\n");
        return;
    }
    List *t = liste;
    while (t != NULL)
    {
        printf("%d -> ", t->val);
        t = t->next;
    }
    printf("NULL\n");
    return;
}

void delel(struct List *liste, int rech)
{
    List *t = liste;
    while(t->val != NULL)
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

    List *t = liste;
    for (int i = 0; i < rech; i++)
    {
        t = t->next;
    }
    return t->val;
}

void lchangeval(struct List *liste, int rech, int newval)
{
    List *t = liste;
    for (int i = 0; i < rech; i++)
    {
        t = t->next;
    }
    t->val = newval;
    return;
}