#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include<time.h> 


void do_everything(void)
{
    /* =================================== Cria o grafo ================================ */

    Graph *g = create_graph("mapa.txt");


    /* ==================== Constroi as Listas de Adjacencias de cada Nó =============== */

    insert_vertices_neighbors(g);    


    /* ======================== Mostra Adjacencias de Cada Vertice ===================== */

    //show_graph_vertices(g);


    /* ======================== Verifica se Todas as Arestas Existem =================== */
    
    if (verify_graph_edges_amount(g)) 
        printf("Esta tudo OK com as arestas\n");
    else
        printf("Teve algum erro!\n"); 


    /* OBSERVAÇÃO: Mostro o CMC onde todos os vértices percorridos são 
                    indicados com um (_) no lugar do símbolo original (M, R ou .),
                    preservando apenas os símbolos I e F.
    */


    /* ======================= Algoritmo Dijkstra - Caminho Mais Curto ================= */

    dijkstra_shortest_path(g);
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


int binary_search(int searched_val, int *lst, int lst_len)
{
    int start, end, midle;
    start = 0; end = lst_len-1; 

    while (start >= end)
    {
        midle = (start + end)/2;

        if (searched_val == lst[midle])  
            return 1;

        if (searched_val > lst[midle])  
            start = ++midle;

        else if (searched_val < lst[midle])  
            end = --midle;
    }    

    return 0;
}


int linear_search(int searched_val, int *lst, int lst_len)
{
    for (int i = 0; i < lst_len; i++)
        if (searched_val == lst[i])
            return 1;
    return 0;
}


void cmp_linar_binary_search(void)
{
    int vet_len = 1000000;
    int *vet = (int *) malloc(vet_len * sizeof(int));

    for(int i = 0; i < vet_len; i++) 
        vet[i] = i;

    
    clock_t t; 
    double time_taken;
    int searched_num = vet[vet_len-1];


    printf("numero buscado:%d\n\n", searched_num);



    printf("======= linear search for the number %d in vet ===========\n", searched_num);
    t = clock(); 
    if (linear_search(searched_num, vet, vet_len))
        printf("encontrei o numero\n");
    t = clock() - t; 
    time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds 
    printf("took %.30f seconds to execute \n", time_taken); 


    printf("\n======= binary search for the number %d in vet ===========\n", searched_num);
    t = clock(); 
    if (binary_search(searched_num, vet, vet_len))
        printf("encontrei o numero\n");
    t = clock() - t; 
    time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds 
    printf("took %.30f seconds to execute \n", time_taken); 
    

    free(vet);
}




int main (void)
{
    //do_everything();

    cmp_linar_binary_search();
    
    printf("hello world\n");

    return 0;
}


