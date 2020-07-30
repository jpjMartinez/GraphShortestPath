#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "graph.h"
#include "fila.h"


struct graph
{
    int num_v;
    Vert **list_vert;  
};


struct vertice
{
    char info;
    int neighbors[4]; /* cima, baixo, esq, dir */
};


Graph * create_graph(int num_vertices, char matriz[40][82])
{
    int i, j, k;

    Graph *g = (Graph *) malloc(sizeof(Graph));
    Vert **lst_vert = (Vert **) malloc(num_vertices * sizeof(Vert *));

    if (g == NULL || lst_vert == NULL) { exit(1); }

    g->num_v = num_vertices;   
    g->list_vert = lst_vert;

    j = 0; k = 0;

    for (i = 0; i < g->num_v; i++)
    {
        g->list_vert[i] = create_vertice(matriz[j][k]);
        
        if (k + 1 == 82)
        {
            k = 0;
            j++;
        }

        else { k++; }            
    }
        
    return g;
}


Vert * create_vertice(char info)
{
    Vert *v = (Vert *) malloc(sizeof(Vert));   
    if (v == NULL) { exit(1); }

    v->info = info;

    int i;
    for (i = 0; i < 4; i++)
        v->neighbors[i] = -1;

    return v;
}


void show_graph_vertices(Graph *g)
{
    int i, j, index_neighbor;
    char pos[4][6] = {"up", "down", "left", "right"};


    for (i = 0; i < g->num_v; i++)
    {
        printf("pointer: %d - info: %c - 0x%X\n", i, g->list_vert[i]->info, g->list_vert[i]);
        
        for (j = 0; j < 4; j++)
        {
            index_neighbor = g->list_vert[i]->neighbors[j];

            if (index_neighbor == -1)
                printf("%s: NULL > ", pos[j]);
            else         
                printf("%s: 0x%X > ", pos[j], g->list_vert[index_neighbor]);                
        }

        printf("\n\n");
    }
}


void insert_vertices_neighbors(Graph *g, char matriz[40][82])
{
    int i, j, k, cont;
    int matrix_aux[40][82];

    cont = 0;

    for (i = 0; i < 40; i++)
        for (j = 0; j < 82; j++)
            matrix_aux[i][j] = cont++;       


    j = 0; k = 0; 


    for (i = 0; i < g->num_v; i++)
    {   
        /* --------------- Define o CIMA do vértice ------------------ */

        if (j - 1 >= 0)
            g->list_vert[i]->neighbors[0] = matrix_aux[j-1][k];     


        /* --------------- Define o BAIXO do vértice ------------------ */

        if (j + 1 < 40)
            g->list_vert[i]->neighbors[1] = matrix_aux[j+1][k]; 


        /* --------------- Define o ESQUERDA do vértice ------------------ */

        if (k - 1 >= 0)
            g->list_vert[i]->neighbors[2] = matrix_aux[j][k-1];    


        /* --------------- Define o DIREITA do vértice ------------------ */

        if (k + 1 < 82)
            g->list_vert[i]->neighbors[3] = matrix_aux[j][k+1];                     
     
        
        if (k + 1 == 82)
        {
            k = 0;
            j++;
        }

        else { k++; }            
    }
}


int verify_graph_edges_amount(Graph *g, int rows, int cols)
{
    int i, j;
    int exepected_pos[4];

    exepected_pos[0] = cols*(rows - 1);
    exepected_pos[1] = exepected_pos[0];
    exepected_pos[2] = rows*(cols - 1);
    exepected_pos[3] = exepected_pos[2];

    int qtd_pos_found[4] = {0, 0, 0, 0};
                        /*up, down, left, right */

    for (i = 0; i < g->num_v; i++)
        for (j = 0; j < 4; j++)
            if (g->list_vert[i]->neighbors[j] != -1)
                qtd_pos_found[j]++;

    if (qtd_pos_found[0] == exepected_pos[0] &&
        qtd_pos_found[1] == exepected_pos[1] &&
        qtd_pos_found[2] == exepected_pos[2] &&
        qtd_pos_found[3] == exepected_pos[3])

        return 1;

    return 0;    
}


void free_vertice(Vert *v)
{
    free(v);
}


void free_graph(Graph *g)
{
    int i;
    for (i = 0; i < g->num_v; i++)
        free_vertice(g->list_vert[i]);

    free(g);
}


int is_a_shortest_path_vert(int vert, int *lst_shortest_path_vertices, int lst_len)
{
    int i;
    for (i = 0; i < lst_len; i++)
        if (vert == lst_shortest_path_vertices[i])
            return 1;
    return 0;
}


void show_shortest_path_DJKT_A_star(Graph *g, int *parents, int end, int origin, char matrix[40][82])
{
    int i, j, cont, parent, lst_parents_len;
    int matrix_aux[40][82];
    int *lst_parents;
    

    cont = 0;

    /* Preenche matriz auxiliar com numeros de 0 à 3279
       para mapear os elementos percorridos no CMC
    */

    for (i = 0; i < 40; i++)
        for (j = 0; j < 82; j++)
            matrix_aux[i][j] = cont++;


    /* Descobre quem são os parentes e coloca eles em fila */

    Fila *f = create_fila();
    cont = 0;

    parent = parents[end];

    while(parent != origin)
    {
        insert_fila(f, parent);
        parent = parents[parent];
        cont++;
    }

    /* Aloca um vetor apenas para os parentes que foram 
       enfileirados anteriormente
    */

    lst_parents = (int *) malloc(cont * sizeof(int)); 
    if (lst_parents == NULL) { exit(1); }
    lst_parents_len = cont;

    cont = 0; 
    
    while (!fila_is_empty(f)) /* Preenche o vetor lst_parents */
    {
        lst_parents[cont] = remove_fila(f);
        cont++;
    }


    printf("==================== CAMINHO PERCORRIDO =====================\n");

    for (i = 0; i < 40; i++)
    {
        for (j = 0; j < 82; j++)
        {
            if (is_a_shortest_path_vert(matrix_aux[i][j], lst_parents, lst_parents_len)) // Mostra X para os Vértices Percorridos
            {
                if (j + 1 == 82)
                    printf("_ \n");
                else
                    printf("_ ");

                remove_fila(f);
            }

            else // Mostra o simbolo normal dos Vértices Não Percorridos 
            {
                if (j + 1 == 82)
                    printf("%c \n", matrix[i][j]);
                else
                    printf("%c ", matrix[i][j]);                
            }
        }
    }       


    /* libera o espaço alocado anteriormente para a fila e lista de 'parents'  */
    free_fila(f);
    free(lst_parents);
}


int calculate_edge_cost(char info)
{
    if (info == 'M')
        return 100;
    
    else if (info == 'R')
        return 5;

    else if (info == '.')
        return 1;

    else if (info == 'F' || info == 'I')
        return 0;

    else
    {
        printf("Mapa possui simbolos diferentes de 'M', 'R' e '.'");
        exit(1);
    }    
} 


void rec_dfs_dijkstra(int origin, Graph *g, int *steps, int *visited, int *distances, int *parents)
{
    int i, neighbor_index, edge_cost, next_vertice;
    int shortest_added_distance = INT_MAX;
    visited[origin] = 1;  
    (*steps)++;


    for (i = 0; i < 4; i++)
    {
        neighbor_index = g->list_vert[origin]->neighbors[i];
        
        if (neighbor_index != -1 && visited[neighbor_index] == 0)
        {
            edge_cost = calculate_edge_cost(g->list_vert[neighbor_index]->info);

            if (edge_cost + distances[origin] < distances[neighbor_index])
            {
                distances[neighbor_index] = edge_cost + distances[origin];
                parents[neighbor_index] = origin;
            }                
        }        
    } 


    /* Descobre qual é a menor distancia acumulada até o momento */

    for (i = 0; i < g->num_v; i++)
    {
        if (distances[i] < shortest_added_distance && visited[i] == 0)
        {
            shortest_added_distance = distances[i];
            next_vertice = i;
        }
    }

    /* Caso shortest_added_distance seja igual a INT_MAX, significa 
        que todos os vértices 'furaram' a condição de visited[i] == 0 no 
        for acima. Assim, todos eles já foram visitados, o que representa 
        uma condição de parada para o algoritmo de Dijkstra
    */
    
    if (shortest_added_distance != INT_MAX)
        rec_dfs_dijkstra(next_vertice, g, steps, visited, distances, parents);          
}


void dijkstra_shortest_path(Graph *g, char matrix[40][82])
{   
    /* Inicia a Contagem de Tempo da Execução da Função */

    clock_t t = clock();

    int i, origin, end;
    int visited[g->num_v];
    int distances[g->num_v];
    int parents[g->num_v]; 
    int steps = 0;


    for (i = 0; i < g->num_v; i++)
    {
        if (g->list_vert[i]->info == 'I') {  origin = i; }
        else if (g->list_vert[i]->info == 'F') { end = i; }
        distances[i] = INT_MAX;
        visited[i] = 0;
    }       
    
    distances[origin] = 0;
    parents[origin] = -1;
  

    rec_dfs_dijkstra(origin, g, &steps, visited, distances, parents);


    /* Calcula o tempo usado para encontrar o CMC */

    t = clock() - t;

    printf("\n================== Consegui rodar o Dijkstra!!! =====================\n");
    printf("Custo do Caminho Mais Curto (CMC): %d\n", distances[end]);
    printf("Numero de vertices visitados: %d\n", steps);
    printf("Tempo em %f segundos\n\n", ((double)t)/CLOCKS_PER_SEC);

    
    /* Mostra o CMC encontrado pelo algoritmo */

    show_shortest_path_DJKT_A_star(g, parents, end, origin, matrix);
}


int calculate_heuristic(int neighbor, int destiny)
{
    int coord[4]; /* neighbor_x, destiny_x, neighbor_y, destiny_y */
    int coord_complete = 0;

    int matrix_aux[40][82];
    int i, j;
    int cont = 0;

    /* Cria matriz auxiliar para mapear os valores de neighbor e destiny
       e encontrar suas coordenadas, necessárias para o cálculo heurístico
    */

    for (i = 0; i < 40; i++) 
        for (j = 0; j < 82; j++)
            matrix_aux[i][j] = cont++;


    for (i = 0; i < 40; i++) 
    {
        for (j = 0; j < 82; j++)
        {
            if (coord_complete == 4) /* já encontrou tds as coordenadas necessárias */
                break;

            if (matrix_aux[i][j] == neighbor)
            {
                coord[0] = i; /* neighbor_x */
                coord[2] = j; /* neighbor_y */
                coord_complete++; coord_complete++;
            }

            else if (matrix_aux[i][j] == destiny)
            {
                coord[1] = i; /* destiny_x */
                coord[3] = j; /* destiny_y */
                coord_complete++; coord_complete++;
            }
        }
    }

    return abs(coord[0] - coord[1]) + abs(coord[2] - coord[3]);
}


void rec_dfs_a_star(int origin, int end, Graph *g, int *steps, int *visited, int *distances, int *parents)
{
    int i, neighbor_index, g_n, h_n, next_vertice;
    int shortest_added_distance = INT_MAX;
    visited[origin] = 1;  
    (*steps)++;


    for (i = 0; i < 4; i++)
    {
        neighbor_index = g->list_vert[origin]->neighbors[i];
        
        if (neighbor_index != -1 && visited[neighbor_index] == 0)
        {
            g_n = calculate_edge_cost(g->list_vert[neighbor_index]->info);
            h_n = calculate_heuristic(neighbor_index, end);

            if ((g_n + distances[origin]) + h_n < distances[neighbor_index])
            {
                distances[neighbor_index] = g_n + distances[origin];
                parents[neighbor_index] = origin;
            }                
        }        
    } 


    /* Descobre qual é a menor distancia acumulada até o momento */

    for (i = 0; i < g->num_v; i++)
    {
        if (distances[i] < shortest_added_distance && visited[i] == 0)
        {
            shortest_added_distance = distances[i];
            next_vertice = i;
        }
    }


    /* Se a heurística der 0, então o destino já foi alcançado.
       Caso contrário, segue adiante com a recursão */
    
    if (h_n != 0) 
        rec_dfs_a_star(next_vertice, end, g, steps, visited, distances, parents);    
}


void a_star_shortest_path(Graph *g, char matrix[40][82])
{
    /* Inicia a Contagem de Tempo da Execução da Função */

    clock_t t = clock();

    int i, origin, end;
    int visited[g->num_v];
    int distances[g->num_v];
    int parents[g->num_v]; 
    int steps = 0;


    for (i = 0; i < g->num_v; i++)
    {
        if (g->list_vert[i]->info == 'I') {  origin = i; }
        else if (g->list_vert[i]->info == 'F') { end = i; }
        distances[i] = INT_MAX;
        visited[i] = 0;
    }       
    
    distances[origin] = 0;
    parents[origin] = -1;
  

    rec_dfs_a_star(origin, end, g, &steps, visited, distances, parents);


    /* Calcula o tempo usado para encontrar o CMC */

    t = clock() - t;

    printf("\n================== Consegui rodar o A * !!! =====================\n");
    printf("Custo do Caminho Mais Curto (CMC): %d\n", distances[end]);
    printf("Numero de vertices visitados: %d\n", steps);
    printf("Tempo em %f segundos\n\n", ((double)t)/CLOCKS_PER_SEC);


    /* Mostra o CMC encontrado pelo algoritmo */

    show_shortest_path_DJKT_A_star(g, parents, end, origin, matrix);
}


int verify_vertice_neighborhood(Graph *g, int vertice, int possible_neighbor)
{
    int i;
    for (i = 0; i < 4; i++)
        if (g->list_vert[vertice]->neighbors[i] == possible_neighbor)
            return 1;
    return 0;
}


void show_shortest_path_FW(int **next_vert_matrix, int origin, int end, char matrix[40][82])
{ 
    if (next_vert_matrix[origin][end] == -1) 
        return ;
              

    int *lst_parents;
    int lst_parents_len, i, j;
    int matrix_aux[40][82];
    int cont = 0;


    /* Preenche matriz auxiliar com numeros de 0 à 3279
       para mapear os elementos percorridos no CMC
    */

    for (i = 0; i < 40; i++)
        for (j = 0; j < 82; j++)
            matrix_aux[i][j] = cont++;


    /* Insere na fila os vértices (parents) serão usados pra construir o CMC */
    
    Fila *f = create_fila();

    /* Não inseri a origem de propósito, pois queria mostar o 'I' como 
        vértice inicial do CMC percorrido
    */

    while (origin != end) 
    { 
        origin = next_vert_matrix[origin][end]; 

        /* Fiz isso, pois queria mostrar o 'F' como vértice final do CMC percorrido */        
        if (origin == end) { break; }

        insert_fila(f, origin);
        cont++;
    }


    /* Aloca um vetor apenas para os parentes que foram 
       enfileirados anteriormente
    */

    lst_parents = (int *) malloc(cont * sizeof(int)); 
    if (lst_parents == NULL) { exit(1); }
    lst_parents_len = cont;

    cont = 0; 
    
    while (!fila_is_empty(f)) /* Preenche o vetor lst_parents */
    {
        lst_parents[cont] = remove_fila(f);
        cont++;
    }


    printf("==================== CAMINHO PERCORRIDO =====================\n");

    for (i = 0; i < 40; i++)
    {
        for (j = 0; j < 82; j++)
        {
            if (is_a_shortest_path_vert(matrix_aux[i][j], lst_parents, lst_parents_len)) // Mostra X para os Vértices Percorridos
            {
                if (j + 1 == 82)
                    printf("_ \n");
                else
                    printf("_ ");

                remove_fila(f);
            }

            else // Mostra o simbolo normal dos Vértices Não Percorridos 
            {
                if (j + 1 == 82)
                    printf("%c \n", matrix[i][j]);
                else
                    printf("%c ", matrix[i][j]);                
            }
        }
    }   


    /* libera o espaço alocado anteriormente para a fila e lista de 'parents'  */
    free_fila(f);
    free(lst_parents);
}


void floyd_warshall_shortest_path(Graph *g, char matrix[40][82])
{
    /* Inicia a Contagem de Tempo da Execução da Função */

    clock_t t = clock();

    int **dist_matrix, **next_vert_matrix;
    int *dist_matrix_column, *next_vert_matrix_column;
    int i, j, k, origin, end;
    float steps = 0;

        
    dist_matrix = (int **) malloc(g->num_v * sizeof(int *));
    next_vert_matrix = (int **) malloc(g->num_v * sizeof(int *));
    if (dist_matrix == NULL || next_vert_matrix == NULL) { exit(1); }


    for (i = 0; i < g->num_v; i++)
    {
        dist_matrix_column = (int *) malloc(g->num_v * sizeof(int));
        next_vert_matrix_column = (int *) malloc(g->num_v * sizeof(int));
        if (dist_matrix_column == NULL || next_vert_matrix_column == NULL) { exit(1); }

        dist_matrix[i] = dist_matrix_column;
        next_vert_matrix[i] = next_vert_matrix_column;
    }
        

    /* Converte a Lista de Adjacencias em uma Matriz de Adjacencias-Distancias 
       Ao mesmo tempo preenche a matriz auxiliar next_vert_matrix para dps recuperar
       os nós percorrido no CMC
    */
    
    for (i = 0; i < g->num_v; i++)
    {
        for (j = 0; j < g->num_v; j++)
        {
            if (i == j)
            {
                dist_matrix[i][j] = 0;
                next_vert_matrix[i][j] = j;
            }            

            else if (verify_vertice_neighborhood(g, i, j))
            {
                dist_matrix[i][j] = calculate_edge_cost(g->list_vert[j]->info);
                next_vert_matrix[i][j] = j;
            }                

            else
            {
                dist_matrix[i][j] = INT_MAX;     
                next_vert_matrix[i][j] = -1;
            }                       
        }
    }           


    /* Atualiza as distancias minimas de pares de nó (i,j) 
        com relação aos 1,2..k-1 vértices do grafo */

    for (k = 0; k < g->num_v; k++) 
        for (i = 0; i < g->num_v; i++) 
            for (j = 0; j < g->num_v; j++) 
                if (dist_matrix[i][k] != INT_MAX && dist_matrix[k][j] != INT_MAX && 
                    dist_matrix[i][k] + dist_matrix[k][j] < dist_matrix[i][j]) 
                    {
                        dist_matrix[i][j] = dist_matrix[i][k] + dist_matrix[k][j]; 
                        next_vert_matrix[i][j] = next_vert_matrix[i][k];
                        steps++;
                    }
                    

    /* Calcula o tempo usado para encontrar o CMC */

    t = clock() - t;


    for (i = 0; i < g->num_v; i++) /* Encontra a origem e o destino */
    {
        if (g->list_vert[i]->info == 'I') {  origin = i; }
        else if (g->list_vert[i]->info == 'F') { end = i; }
    }

    printf("\n================== Consegui rodar o Floyd-Warshall!!! =====================\n");
    printf("Custo do Caminho Mais Curto (CMC): %d\n", dist_matrix[origin][end]);
    printf("Numero de vertices visitados: %.0f\n", steps);
    printf("Tempo em %f segundos\n\n", ((double)t)/CLOCKS_PER_SEC);


    /* Mostra caminho mais curto */

    show_shortest_path_FW(next_vert_matrix, origin, end, matrix);



    /* Libera a memoria usada na Matriz de Adjacencias-Distancias 
       e na matriz usada para remontar o shortest path 
    */

    for (i = 0; i < g->num_v; i++)
    {
        free(dist_matrix[i]);
        free(next_vert_matrix[i]);
    }
        
    free(dist_matrix);
    free(next_vert_matrix);
}