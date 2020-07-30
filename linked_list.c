#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"


LinkedList * create_linked_list(void)
{
    return NULL;
}


LinkedList * end_insert_linked_list(LinkedList *l, int info)
{
    LinkedList *new_node = (LinkedList *) malloc(sizeof(LinkedList));
    if (new_node == NULL) { exit(1); }

    new_node->prox = NULL;
    new_node->info = info;

    if (l != NULL)
        l->prox = new_node;      

    l = new_node;

    return l;
}


void free_linked_list(LinkedList *l)
{
    if (l != NULL)
        free_linked_list(l->prox);
        free(l);
}


LinkedList * start_remove_linked_list(LinkedList *l, int *info_retrieved)
{
    LinkedList *prox_aux = l->prox;
    *info_retrieved = l->info;

    free(l);
    l = prox_aux;

    return l;    
}


void print_linked_list(LinkedList *l)
{
    if (l != NULL)
    {
        if (l->prox == NULL)
            printf("%d (End)\n\n", l->info);
        else
            printf("%d > ", l->info);
        
        print_linked_list(l->prox);
    }
}