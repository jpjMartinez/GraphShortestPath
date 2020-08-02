typedef struct graph Graph;
typedef struct vertice Vert;

Graph * create_graph(char *map_file);
void free_graph(Graph *g);

Vert * create_vertice(char info);
void free_vertice(Vert *v);

void insert_vertices_neighbors(Graph *g);
void show_graph_vertices(Graph *g);
int verify_graph_edges_amount(Graph *g);

int is_a_shortest_path_vert(int vert, int *lst_shortest_path_vertices, int len);

void show_shortest_path_DJKT_A_star(Graph *g, int *parents, int end, int origin);

int calculate_edge_cost(char info);
void rec_dfs_dijkstra(Graph *g, int origin, int *steps, int *visited, int *distances, int *parents);
void dijkstra_shortest_path(Graph *g);

int calculate_heuristic(int neighbor, int destiny, int rows, int cols);
void rec_dfs_a_star(Graph *g, int origin, int end, int *steps, int *visited, int *distances, int *parents);
void a_star_shortest_path(Graph *g);



int verify_vertice_neighborhood(Graph *g, int vert_index, int possible_neighbor);
void show_shortest_path_FW(int **next_vert_matrix, int origin, int end, char matrix[40][82]);
void floyd_warshall_shortest_path(Graph *g, char matrix[40][82]);


