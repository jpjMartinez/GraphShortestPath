typedef struct linked_list LinkedList;

struct linked_list
{
    int info;
    LinkedList *prox;
};

LinkedList * create_linked_list(void);

LinkedList * end_insert_linked_list(LinkedList *l, int info);

LinkedList * start_remove_linked_list(LinkedList *l, int *info_retrieved);

void free_linked_list(LinkedList *l);

void print_linked_list(LinkedList *l);