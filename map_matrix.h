typedef struct map_matrix MapMatrix;

MapMatrix *create_map_matrix(char *map_file);
void free_map_matrix(MapMatrix *map_matrix);

char **create_matrix(int *rows, int *cols, char *map_file);
int **create_matrix_aux(int *rows, int *cols);

void print_map_matrix(MapMatrix *map_matrix);

void get_map_matrix_dimensions(MapMatrix *map_matrix);
int get_matrix_cols(MapMatrix *map_matrix);
int get_matrix_rows(MapMatrix *map_matrix);

char **get_matrix(MapMatrix *map_matrix);
int **get_matrix_aux(MapMatrix *map_matrix);