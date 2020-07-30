#include <stdio.h>
#include <stdlib.h>
#include "map_matrix.h"


struct map_matrix
{
    char **matrix;
    int rows;
    int cols;
};


MapMatrix *create_map_matrix(char *map_file)
{
    MapMatrix *map_matrix = (MapMatrix *) malloc(sizeof(MapMatrix));
    if (map_matrix == NULL) { exit(1); }

    map_matrix->matrix = create_matrix(&(map_matrix->rows), &(map_matrix->cols), map_file);

    return map_matrix;
}


char **create_matrix(int *rows, int *cols, char *map_file)
{
    FILE *map;
    char ch;
    char **map_matrix;
    int i, j;

    *rows = 0; *cols = 0;
       

    /* Determinar as dimensões do mapa */

    map = fopen(map_file, "r");
    if (map == NULL) { exit(1); }          

    while ((ch = fgetc(map)) != EOF)
    {
        if (ch != '\n')
            (*cols)++; /* usado para contar TODOS os vertices do mapa */

        else
            (*rows)++; /* conta as linhas da matrix, normalmente */                                  
    }

    fclose(map);

    (*rows)++; // soma 1 pq já estava na última linha, então n tinha um '\n' 
    *cols = (*cols) / *rows;



    /* Controi a matriz que representa o mapa */

    map_matrix = (char **) malloc((*rows) * sizeof(char *)); // vet ponteiros char 
    if (map_matrix == NULL) { exit(1); }

    for (i = 0; i < (*rows); i++)
    {
        map_matrix[i] = (char *) malloc((*cols) * sizeof(char));
        if (map_matrix[i] == NULL) { exit(1); }
    }
    


    /* Preenchee a matriz-mapa construida */

    map = fopen(map_file, "r");
    if (map == NULL) { exit(1); }          

    for (i = 0; i < (*rows); i++)
        for (j = 0; j < (*cols); j++)
        {
            ch = fgetc(map);
            if (ch == '\n')
                ch = fgetc(map);
            
            map_matrix[i][j] = ch;
        }
            
    fclose(map);
    


    return map_matrix;
}


void free_map_matrix(MapMatrix *map_matrix)
{
    for (int i = 0; i < map_matrix->rows; i++)
        free(map_matrix->matrix[i]);

    free(map_matrix->matrix);

    free(map_matrix);
}


void print_map_matrix(MapMatrix *map_matrix)
{
    /* Mostra a matriz-mapa preenchida */

    for (int i = 0; i < map_matrix->rows; i++)
        for (int j = 0; j < map_matrix->cols; j++)
        {
            if (j + 1 == map_matrix->cols)
                printf("%c \n", map_matrix->matrix[i][j]);
            else
                printf("%c ",  map_matrix->matrix[i][j]);
        }
}


void get_map_matrix_dimensions(MapMatrix *map_matrix)
{
    printf("\n\nrows:%d - cols:%d\n\n", map_matrix->rows, map_matrix->cols);
}