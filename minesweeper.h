#include "params.h"

struct cell {
    int mined;
    int selected;
    int num_adjacent;
    struct cell *adjacent[MAX_ADJACENT];
    int coords[MAX_DIM];
    int hint;
};

void print_game(struct cell *game, int dim, int *dim_sizes);

void init_game(struct cell *game, int dim, int *dim_sizes, int num_mines, int **mined_cells);

int select_cell(struct cell *game, int dim, int *dim_sizes, int *coords);

void select_recursion(struct cell *c);

int get_num_cells(int dim, int *dim_sizes);

int check_win(struct cell *game, int dim, int *dim_sizes);

int is_adjacent(int dim, int *coords1, int *coords2);

int coords_to_index(int dim, int *dim_sizes, int *coords);

void index_to_coords(int *coords, int index, int dim, int *dim_sizes);
