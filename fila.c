#include <stdlib.h>
#include "fila.h"
#include "linked_list.h"


struct fila
{
    LinkedList *origin;
    LinkedList *end;
};


Fila * create_fila(void)
{
    Fila *f = (Fila *) malloc(sizeof(Fila));
    LinkedList *lst_nodes = create_linked_list();

    if (f == NULL) { exit(1); }

    f->origin = lst_nodes;
    f->end = lst_nodes;

    return f;
}


void insert_fila(Fila *f, int info)
{
    f->end = end_insert_linked_list(f->end, info);

    if (f->origin == NULL) /* A fila estava vazia  */       
        f->origin = f->end;        
}


int remove_fila(Fila *f)
{
    /* Erro. Tenta Tirar Elemento de uma Fila Vazia */

    if (f->origin == NULL && f->end == NULL) { return -1; } 
    
    int info_retrieved;
    f->origin = start_remove_linked_list(f->origin, &info_retrieved);

    if (f->origin == NULL) /* A fila ficou vazia */
        f->end = NULL;

    return info_retrieved;
}


void free_fila(Fila *f)
{
    free_linked_list(f->origin);
    free(f);
}


void print_fila_elements(Fila *f)
{
    print_linked_list(f->origin);
}


int get_fila_topo(Fila *f)
{
    return f->origin->info;
}


int fila_is_empty(Fila *f)
{
    return (f->origin == NULL && f->end == NULL);
}