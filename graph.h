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

void show_shortest_path_DJKT_A_star(Graph *g, int *parents, int end, int origin, char matrix[40][82]);

int calculate_edge_cost(char info);

void rec_dfs_dijkstra(int origin, Graph *g, int *steps, int *visited, int *distances, int *parents);

void dijkstra_shortest_path(Graph *g, char matrix[40][82]);

int calculate_heuristic(int neighbor, int destiny);

void rec_dfs_a_star(int origin, int end, Graph *g, int *steps, int *visited, int *distances, int *parents);

void a_star_shortest_path(Graph *g, char matrix[40][82]);

int verify_vertice_neighborhood(Graph *g, int vertice, int possible_neighbor);

void show_shortest_path_FW(int **next_vert_matrix, int origin, int end, char matrix[40][82]);

void floyd_warshall_shortest_path(Graph *g, char matrix[40][82]);