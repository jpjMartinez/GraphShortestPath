#include <stdio.h>
#include <stdlib.h>
#include "graph.h"


void make_everything(void)
{
    /*============ Le o arquivo mapa.txt e o transforma em uma matriz 40x82 ============ */

    FILE *map;
    char ch;
    int row = 0;
    int col = 0;
    char matrix[40][82];


    map = fopen("mapa.txt", "r");
    if (map == NULL) { exit(1); }          

    while ((ch = fgetc(map)) != EOF)
    {
        if (ch != '\n')
        {
            matrix[row][col] = ch;
            col++;
        }   

        else
        {
            row++;
            col = 0;
        }                                   
    }

    fclose(map);


    /* =================================== Cria o grafo ================================ */

    Graph *g = create_graph(3280, matrix);


    /* ==================== Constroi as Listas de Adjacencias de cada Nó =============== */

    insert_vertices_neighbors(g, matrix);


    /* ======================== Verifica se Todas as Arestas Existem =================== */
    
    if (verify_graph_edges_amount(g, 40, 82)) 
        printf("Esta tudo OK com as arestas\n");
    else
        printf("Teve algum erro!\n"); 


    /* OBSERVAÇÃO: Mostro o CMC onde todos os vértices percorridos são 
                    indicados com um (_) no lugar do símbolo original (M, R ou .),
                    preservando apenas os símbolos I e F.
    */


    /* ======================= Algoritmo Dijkstra - Caminho Mais Curto ================= */

    dijkstra_shortest_path(g, matrix);
    printf("\n\n");


    /* ========================== Algoritmo A* - Caminho Mais Curto ==================== */

    //a_star_shortest_path(g, matrix);
    printf("\n\n");


    /* ==================== Algoritmo Floyd-Warshall - Caminho Mais Curto ============== */

    //floyd_warshall_shortest_path(g, matrix);
    printf("\n\n");
    

    /* ================================= Destroi o grafo =============================== */

    free_graph(g);
}




char **create_map_matrix(int *rows, int *cols)
{
    FILE *map;
    char ch;
    char **map_matrix;
    int i, j;

    *rows = 0; *cols = 0;
       

    /* Determinar as dimensões do mapa */

    map = fopen("mapa.txt", "r");
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

    map = fopen("mapa.txt", "r");
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


void free_map_matrix(char **map_matrix, int rows)
{
    for (int i = 0; i < rows; i++)
        free(map_matrix[i]);

    free(map_matrix);
}


void print_map_matrix(char **map_matrix, int rows, int cols)
{
    /* Mostra a matriz-mapa preenchida */

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
        {
            if (j + 1 == cols)
                printf("%c \n", map_matrix[i][j]);
            else
                printf("%c ",  map_matrix[i][j]);
        }
}




int main (void)
{
    //make_everything();

    int rows, cols;
    char **map_matrix;
    map_matrix = create_map_matrix(&rows, &cols);

    print_map_matrix(map_matrix, rows, cols);
    printf("\n\nrows:%d - cols:%d\n\n", rows, cols);

    free_map_matrix(map_matrix, rows);
    printf("hello world\n");

    return 0;
}


