#include <stdio.h>
#include <stdlib.h>
#include "map_matrix.h"


struct map_matrix
{
    char **matrix;
    int **matrix_aux;
    int rows;
    int cols;
};


MapMatrix *create_map_matrix(char *map_file)
{
    MapMatrix *map_matrix = (MapMatrix *) malloc(sizeof(MapMatrix));
    if (map_matrix == NULL) { exit(1); }

    map_matrix->matrix = create_matrix(&(map_matrix->rows), &(map_matrix->cols), map_file);
    map_matrix->matrix_aux = create_matrix_aux(&(map_matrix->rows), &(map_matrix->cols));

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


int **create_matrix_aux(int *rows, int *cols)
{
    int i, j, index;
    int **matrix_aux;
    

    /* Controi a estrutura da matriz auxilar */

    matrix_aux = (int **) malloc((*rows) * sizeof(int *)); // vet ponteiros int 
    if (matrix_aux == NULL) { exit(1); }

    for (i = 0; i < (*rows); i++)
    {
        matrix_aux[i] = (int *) malloc((*cols) * sizeof(int));
        if (matrix_aux[i] == NULL) { exit(1); }
    }


    /* Preenche a matrix auxiliar com os index`s de cada vertice */

    index = 0;

    for (i = 0; i < (*rows); i++)
        for (j = 0; j < (*cols); j++)
            matrix_aux[i][j] = index++;      

    return matrix_aux;
}


void free_map_matrix(MapMatrix *map_matrix)
{
    for (int i = 0; i < map_matrix->rows; i++) 
    {
        free(map_matrix->matrix[i]); // libera o custo de cada vertice (matriz)
        free(map_matrix->matrix_aux[i]); // libera o index de cada vertice (matriz)
    }
        
    free(map_matrix->matrix);
    free(map_matrix->matrix_aux);

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


int get_matrix_rows(MapMatrix *map_matrix)
{
    return map_matrix->rows;
}


int get_matrix_cols(MapMatrix *map_matrix)
{
    return map_matrix->cols;
}


char **get_matrix(MapMatrix *map_matrix)
{
    return map_matrix->matrix;
}


int **get_matrix_aux(MapMatrix *map_matrix)
{
    return map_matrix->matrix_aux;
}


void get_map_matrix_dimensions(MapMatrix *map_matrix)
{
    printf("\n\nrows:%d - cols:%d\n\n", map_matrix->rows, map_matrix->cols);
}