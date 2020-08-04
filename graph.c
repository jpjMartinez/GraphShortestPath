#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "graph.h"
#include "map_matrix.h"
#include "algorithms.h"

 
struct graph
{ 
    int num_v;
    Vert **list_vert;  
    MapMatrix *map_matrix;
};


struct vertice
{
    char info;
    int neighbors[4]; /* cima, baixo, esq, dir */
};


Graph * create_graph(char *map_file)
{
    Graph *g = (Graph *) malloc(sizeof(Graph));   
    MapMatrix *map_matrix = create_map_matrix(map_file);
    if (g == NULL || map_matrix == NULL) { exit(1); }

    g->num_v = get_matrix_rows(map_matrix) * get_matrix_cols(map_matrix);
    g->map_matrix = map_matrix;


    Vert **lst_vert = (Vert **) malloc(g->num_v * sizeof(Vert *));
    if (lst_vert == NULL) { exit(1); }
    
    g->list_vert = lst_vert;
    

    int k = 0;

    for (int i = 0; i < get_matrix_rows(g->map_matrix); i++)
        for (int j = 0; j < get_matrix_cols(g->map_matrix); j++)
            g->list_vert[k++] = create_vertice(get_matrix(map_matrix)[i][j]);
     
    return g;
}


void free_graph(Graph *g)
{
    for (int i = 0; i < g->num_v; i++) // libera cada vértice e suas adjacencias
        free_vertice(g->list_vert[i]);

    free(g->list_vert); // libera a lista de adjacencias que endereçava cada vertice

    free_map_matrix(g->map_matrix); // libera o mapa usado para construir o grafo

    free(g);
}


Vert * create_vertice(char info)
{
    Vert *v = (Vert *) malloc(sizeof(Vert));   
    if (v == NULL) { exit(1); }

    v->info = info;

    for (int i = 0; i < 4; i++)
        v->neighbors[i] = -1;

    return v;
}


void free_vertice(Vert *v)
{
    free(v);
}


void insert_vertices_neighbors(Graph *g)
{
    int j, k;    
    j = 0; k = 0;


    for (int i = 0; i < g->num_v; i++)
    {   
        /* --------------- Define o CIMA do vértice ------------------ */

        if (j - 1 >= 0)
            g->list_vert[i]->neighbors[0] = get_matrix_aux(g->map_matrix)[j-1][k];     


        /* --------------- Define o BAIXO do vértice ------------------ */

        if (j + 1 < get_matrix_rows(g->map_matrix))
            g->list_vert[i]->neighbors[1] = get_matrix_aux(g->map_matrix)[j+1][k]; 


        /* --------------- Define o ESQUERDA do vértice ------------------ */

        if (k - 1 >= 0)
            g->list_vert[i]->neighbors[2] = get_matrix_aux(g->map_matrix)[j][k-1];    


        /* --------------- Define o DIREITA do vértice ------------------ */

        if (k + 1 < get_matrix_cols(g->map_matrix))
            g->list_vert[i]->neighbors[3] = get_matrix_aux(g->map_matrix)[j][k+1];                     
     
        
        if (k + 1 == get_matrix_cols(g->map_matrix))
        {
            k = 0;
            j++;
        }

        else { k++; }            
    }
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


int verify_graph_edges_amount(Graph *g)
{
    int rows, cols;
    int exepected_pos[4];

    rows = get_matrix_rows(g->map_matrix);
    cols = get_matrix_cols(g->map_matrix);

    exepected_pos[0] = cols*(rows - 1);
    exepected_pos[1] = exepected_pos[0];
    exepected_pos[2] = rows*(cols - 1);
    exepected_pos[3] = exepected_pos[2];

    int qtd_pos_found[4] = {0, 0, 0, 0};
                        /*up, down, left, right */

    for (int i = 0; i < g->num_v; i++)
        for (int j = 0; j < 4; j++)
            if (g->list_vert[i]->neighbors[j] != -1)
                qtd_pos_found[j]++;

    if (qtd_pos_found[0] == exepected_pos[0] &&
        qtd_pos_found[1] == exepected_pos[1] &&
        qtd_pos_found[2] == exepected_pos[2] &&
        qtd_pos_found[3] == exepected_pos[3])

        return 1;

    return 0;    
}


int is_a_shortest_path_vert(int vert, int *lst_shortest_path_vertices, int lst_len)
{
    int index = binary_search(vert, lst_shortest_path_vertices, lst_len);
    return (index != -1) ? 1 : 0;    
}


void print_map_and_shortest_path(Graph *g, int *lst, int len_lst, Fila *f)
{
    int rows = get_matrix_rows(g->map_matrix);
    int cols = get_matrix_cols(g->map_matrix);

    char **matrix = get_matrix(g->map_matrix);
    int **matrix_aux = get_matrix_aux(g->map_matrix);       


    printf("==================== CAMINHO PERCORRIDO =====================\n");

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
        {
            /* Mostra '_' para os Vértices Percorridos */

            if (is_a_shortest_path_vert(matrix_aux[i][j], lst, len_lst)) 
            {
                if (j + 1 == cols)
                    printf("_ \n");
                else
                    printf("_ ");

                remove_fila(f);
            }


            /* Mostra o simbolo normal dos Vértices Não Percorridos */

            else 
            {
                if (j + 1 == cols)
                    printf("%c \n", matrix[i][j]);
                else
                    printf("%c ", matrix[i][j]);                
            }
        }
}


void show_shortest_path_DJKT_A_star(Graph *g, int *parents, int end, int origin)
{
    int cont, parent, lst_parents_len, *lst_parents;


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


    /* Preenche o vetor lst_parents */

    cont = 0; 
    
    while (!fila_is_empty(f)) 
        lst_parents[cont++] = remove_fila(f);    


    /* Coloca os elementos do vetor em ordem crescente */

    quick_sort(lst_parents, 0, lst_parents_len-1);


    /* Mostra na tela o mapa original com o Caminho Mais Curto percorrido */

    print_map_and_shortest_path(g, lst_parents, lst_parents_len, f);


    /* Libera o espaço alocado anteriormente para a fila e lista de 'parents' */

    free_fila(f);
    free(lst_parents);
}


void show_shortest_path_FW(Graph *g, int **next_vert_matrix, int origin, int end)
{ 
    if (next_vert_matrix[origin][end] == -1) 
        return ;
              
    int cont, lst_parents_len, *lst_parents;


    /* Insere na fila os vértices (parents) serão usados pra construir o CMC */
    
    Fila *f = create_fila();
    cont = 0;

    /* Não inseri a origem de propósito, pois queria mostar o 'I' como 
        vértice inicial do CMC percorrido
    */

    while (origin != end) 
    { 
        origin = next_vert_matrix[origin][end]; 

        /* Fiz isso, pois queria mostrar o 'F' como vértice final do CMC percorrido */

        if (origin == end) 
            break;

        insert_fila(f, origin);
        cont++;
    }


    /* Aloca um vetor apenas para os parentes que foram enfileirados anteriormente */

    lst_parents = (int *) malloc(cont * sizeof(int)); 
    if (lst_parents == NULL) { exit(1); }
    lst_parents_len = cont;


    /* Preenche o vetor lst_parents */

    cont = 0; 
    
    while (!fila_is_empty(f))  
        lst_parents[cont++] = remove_fila(f);


    /* Coloca os elementos do vetor em ordem crescente */

    quick_sort(lst_parents, 0, lst_parents_len-1);    


    /* Mostra na tela o mapa original com o Caminho Mais Curto percorrido */

    print_map_and_shortest_path(g, lst_parents, lst_parents_len, f);


    /* Libera o espaço alocado anteriormente para a fila e lista de 'parents'  */

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


void rec_dfs_dijkstra(Graph *g, int origin, int *steps, int *visited, int *distances, int *parents)
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
        rec_dfs_dijkstra(g, next_vertice, steps, visited, distances, parents);          
}


void dijkstra_shortest_path(Graph *g)
{   
    /* Inicia a Contagem de Tempo da Execução da Função */

    clock_t t = clock();

    int origin, end;
    int visited[g->num_v];
    int distances[g->num_v];
    int parents[g->num_v]; 
    int steps = 0;


    for (int i = 0; i < g->num_v; i++)
    {
        if (g->list_vert[i]->info == 'I') {  origin = i; }
        else if (g->list_vert[i]->info == 'F') { end = i; }
        distances[i] = INT_MAX;
        visited[i] = 0;
    }       
    
    distances[origin] = 0;
    parents[origin] = -1;
  

    rec_dfs_dijkstra(g, origin, &steps, visited, distances, parents);


    /* Calcula o tempo usado para encontrar o CMC */

    t = clock() - t;

    printf("\n================== Consegui rodar o Dijkstra!!! =====================\n");
    printf("Custo do Caminho Mais Curto (CMC): %d\n", distances[end]);
    printf("Numero de vertices visitados: %d\n", steps);
    printf("Tempo em %f segundos\n\n", ((double)t)/CLOCKS_PER_SEC);

    
    /* Mostra o CMC encontrado pelo algoritmo */

    show_shortest_path_DJKT_A_star(g, parents, end, origin);
}


int calculate_heuristic(int neighbor, int destiny, int rows, int cols, int **matrix_aux)
{    
    /* Usa a matriz auxiliar e mapeia os valores de neighbor e destiny,
       encontrando as coordenadas necessárias para o cálculo heurístico
    */

    int coord[4]; /* neighbor_x, destiny_x, neighbor_y, destiny_y */
    int coord_complete = 0;


    for (int i = 0; i < rows; i++) 
    {
        if (coord_complete == 4) // já encontrou tds as coordenadas necessárias
            break;

        for (int j = 0; j < cols; j++)
        {
            if (coord_complete == 4) // já encontrou tds as coordenadas necessárias
                break;

            if (matrix_aux[i][j] == neighbor)
            {
                coord[0] = i; // neighbor_x 
                coord[2] = j; // neighbor_y 
                coord_complete += 2;
            }

            else if (matrix_aux[i][j] == destiny)
            {
                coord[1] = i; // destiny_x 
                coord[3] = j; // destiny_y 
                coord_complete += 2;
            }
        }
    }

    return abs(coord[0] - coord[1]) + abs(coord[2] - coord[3]);
}


void rec_dfs_a_star(Graph *g, int origin, int end, int *steps, int *visited, int *distances, int *parents)
{
    int i, neighbor_index, g_n, h_n, next_vertice;
    int shortest_added_distance = INT_MAX;
    visited[origin] = 1;  
    (*steps)++;

    int rows = get_matrix_rows(g->map_matrix);
    int cols = get_matrix_cols(g->map_matrix);
    int **matrix_aux = get_matrix_aux(g->map_matrix);


    for (i = 0; i < 4; i++)
    {
        neighbor_index = g->list_vert[origin]->neighbors[i];
        
        if (neighbor_index != -1 && visited[neighbor_index] == 0)
        {
            g_n = calculate_edge_cost(g->list_vert[neighbor_index]->info);
            h_n = calculate_heuristic(neighbor_index, end, rows, cols, matrix_aux);
            
            if ((g_n + distances[origin]) + h_n < distances[neighbor_index])
            {
                distances[neighbor_index] = g_n + distances[origin];
                parents[neighbor_index] = origin;
            }                
        }        
    }


    /* Descobre qual é a menor distancia acumulada até o momento */

    for (i = 0; i < g->num_v; i++)
        if (distances[i] < shortest_added_distance && visited[i] == 0)
        {
            shortest_added_distance = distances[i];
            next_vertice = i;
        }

    
    /* Se ja visitou todos os vertices, então o destino já foi alcançado.
       Caso contrário, segue adiante com a recursão */
    
    if ((*steps) != g->num_v)
        rec_dfs_a_star(g, next_vertice, end, steps, visited, distances, parents);    
          
}


void a_star_shortest_path(Graph *g)
{
    /* Inicia a Contagem de Tempo da Execução da Função */

    clock_t t = clock();

    int origin, end;
    int visited[g->num_v];
    int distances[g->num_v];
    int parents[g->num_v]; 
    int steps = 0;


    for (int i = 0; i < g->num_v; i++)
    {
        if (g->list_vert[i]->info == 'I') {  origin = i; }
        else if (g->list_vert[i]->info == 'F') { end = i; }
        distances[i] = INT_MAX;
        visited[i] = 0;
    }       
    
    distances[origin] = 0;
    parents[origin] = -1;
  

    rec_dfs_a_star(g, origin, end, &steps, visited, distances, parents);
    
    //APAGA ESSE return DEPOIS, POIS SERVE APENAS P/ TESTAR calculate_heuristic()
    //return;

    /* Calcula o tempo usado para encontrar o CMC */

    t = clock() - t;

    printf("\n================== Consegui rodar o A * !!! =====================\n");
    printf("Custo do Caminho Mais Curto (CMC): %d\n", distances[end]);
    printf("Numero de vertices visitados: %d\n", steps);
    printf("Tempo em %f segundos\n\n", ((double)t)/CLOCKS_PER_SEC);


    /* Mostra o CMC encontrado pelo algoritmo */

    show_shortest_path_DJKT_A_star(g, parents, end, origin);
}


int verify_vertice_neighborhood(Graph *g, int vert_index, int possible_neighbor)
{  
    int *lst_neighbors = g->list_vert[vert_index]->neighbors;

    int index = linear_search(possible_neighbor, lst_neighbors, 4);

    return (index != -1) ? 1 : 0;
}


void floyd_warshall_shortest_path(Graph *g)
{
    /* Inicia a Contagem de Tempo da Execução da Função */

    clock_t t = clock();

    int **dist_matrix, **next_vert_matrix;
    int i, j, k, origin, end;
    float steps = 0;

        
    dist_matrix = (int **) malloc(g->num_v * sizeof(int *));
    next_vert_matrix = (int **) malloc(g->num_v * sizeof(int *));
    if (dist_matrix == NULL || next_vert_matrix == NULL) { exit(1); }


    for (i = 0; i < g->num_v; i++)
    {
        dist_matrix[i] = (int *) malloc(g->num_v * sizeof(int));
        next_vert_matrix[i]  = (int *) malloc(g->num_v * sizeof(int));
        if (dist_matrix[i] == NULL || next_vert_matrix[i] == NULL) { exit(1); }
    }
        

    /* Converte a Lista de Adjacencias em uma Matriz de Adjacencias-Distancias 
       Ao mesmo tempo preenche a matriz auxiliar next_vert_matrix para dps recuperar
       os nós percorridos no CMC
    */
    
    for (i = 0; i < g->num_v; i++)
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


    for (i = 0; i < g->num_v; i++) // Encontra a origem e o destino 
    {
        if (g->list_vert[i]->info == 'I') {  origin = i; }
        else if (g->list_vert[i]->info == 'F') { end = i; }
    }

    printf("\n================== Consegui rodar o Floyd-Warshall!!! =====================\n");
    printf("Custo do Caminho Mais Curto (CMC): %d\n", dist_matrix[origin][end]);
    printf("Numero de vertices visitados: %.0f\n", steps);
    printf("Tempo em %f segundos\n\n", ((double)t)/CLOCKS_PER_SEC);


    /* Mostra caminho mais curto */

    show_shortest_path_FW(g, next_vert_matrix, origin, end);



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