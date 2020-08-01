#include <stdio.h>
#include <stdlib.h>
#include "graph.h"



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



int main (void)
{
    do_everything();
    
    printf("hello world\n");

    return 0;
}


