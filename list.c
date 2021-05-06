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

void append(List *list, int nv)
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

int pop(List *liste)
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

void afficherListe(List *liste)
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

void delel(List *liste, int t)
{
    while(liste->val != NULL)
    {
        if (liste->val == t)
        {
            liste = liste->next;
            return;
        }
    }
    return;
}