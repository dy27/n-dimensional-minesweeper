#include "minesweeper.h"
#include <stdlib.h>
#include <stdio.h>

void print_game(struct cell *game, int dim, int *dim_sizes) {
    // Calculate number of cells
    int num_cells = 1;
    for (int d=0; d<dim; d++) {
        num_cells *= dim_sizes[d];
    }

    struct cell *c;
    for (int i=0; i<num_cells; i++) {
        c = &game[i];
        printf("Coords:(%d,%d), Adjacent:%d, Hint:%d, Mined:%d, Selected:%d\n", c->coords[0], c->coords[1], c->num_adjacent, c->hint, c->mined, c->selected);
    }

}

int main() {
	struct cell game[500];
	int dim_sizes[] = {7,7};
	int dim = 2;
	int num_mines = 3;
    int mine1[2] = {4,5};
    int mine2[2] = {5,5};
    int mine3[2] = {6,5};
	int *mined_cells[3] = {mine1, mine2, mine3};

	init_game(game, dim, dim_sizes, num_mines, mined_cells);

	print_game(game, dim, dim_sizes);

    printf("\n");

    int coords[2] = {0,0};
    select_cell(game, dim, dim_sizes, coords);
    print_game(game, dim, dim_sizes);

	return 0;
}
