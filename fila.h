typedef struct fila Fila;

Fila * create_fila(void);

void insert_fila(Fila *f, int info);

int remove_fila(Fila *f);

void free_fila(Fila *f);

void print_fila_elements(Fila *f);

int get_fila_topo(Fila *f);

int fila_is_empty(Fila *f);