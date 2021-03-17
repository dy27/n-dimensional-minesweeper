#include "minesweeper.h"
#include <stdlib.h>

/**
* Initialises the game state based on the given arguments.
*
* @param game Array to store the game cells.
* @param dim Number of dimensions of the game.
* @param dim_sizes Array specifying the size of each dimension of the game.
* @param num_mines Number of mines in the game.
* @param mined_cells Array specifying the coordinates of the mined cells.
*/
void init_game(struct cell *game, int dim, int *dim_sizes, int num_mines, int **mined_cells) {
    // Calculate number of cells.
    int num_cells = get_num_cells(dim, dim_sizes);

    // Set cell coordinates.
    for (int i=0; i<num_cells; i++) {
        index_to_coords(game[i].coords, i, dim, dim_sizes);
        game[i].num_adjacent = 0;
    }

    // Set mined cells.
    int *coords;
    int index;
    for (int n=0; n<num_mines; n++) {
        coords = mined_cells[n];
        index = coords_to_index(dim, dim_sizes, coords);
        game[index].mined = 1;
    }

    // Set adjacent cells by checking all cell pairs and set hint values.
    struct cell *cell1;
    struct cell *cell2;
    for (int i=0; i<num_cells; i++) {
        for (int j=0; j<num_cells; j++) {
            if (i == j) {
                continue;
            }
            cell1 = &game[i];
            cell2 = &game[j];

            /* If two checked cells are adjacent, add the second cell to
            the first cell's adjacent array, and increment its num_adjacent
            value. */
            if (is_adjacent(dim, cell1->coords, cell2->coords)) {
                cell1->adjacent[cell1->num_adjacent] = cell2;
                cell1->num_adjacent++;

                if (cell2->mined) {
                    cell1->hint++;
                }
            }
        }
    }
    return;
}

/**
* Selects the cell at the specified coordinates of the game.
*
* @param game Array of game cells.
* @param dim Number of dimensions of the game.
* @param dim_sizes Array specifying the size of each dimension of the game.
* @param coords The coordinates of the cell being selected.
* @return Value indicating whether the game has been won, lost or neither.
*/
int select_cell(struct cell *game, int dim, int *dim_sizes, int *coords) {
    int index = coords_to_index(dim, dim_sizes, coords);
    struct cell *c = &game[index];

    // Check that the specified coordinates are within bounds.
    for (int i=0; i<dim; i++) {
        if (coords[i] < 0 || coords[i] > dim_sizes[i]) {
            return 0;
        }
    }

    // Select the cell if it is unselected.
    if (c->selected == 1) {
        return 0;
    }
    c->selected = 1;

    // Return a loss if the selected cell is mined.
    if (c->mined) {
        return 1;
    }

    select_recursion(c);

    // Return a win if the win condition is satisfied.
    if (check_win(game, dim, dim_sizes)) {
        return 2;
    }
    return 0;
}

/**
* Recursive function for selecting cells.
*
* @param c Pointer to the selected cell.
*/
void select_recursion(struct cell *c) {
    if (c->hint == 0) {
        for (int n=0; n<(c->num_adjacent); n++) {
            struct cell *c_next = c->adjacent[n];
            if (c_next->selected == 0) {
                c_next->selected = 1;
                select_recursion(c_next);
            }
        }
    }
}

/**
* Computes the required number of game cells from the dimensions of the game.
*
* @param dim Number of dimensions of the game.
* @param dim_sizes Array specifying the size of each dimension of the game.
* @return The number of game cells.
*/
int get_num_cells(int dim, int *dim_sizes) {
    int num_cells = 1;
    for (int d=0; d<dim; d++) {
        num_cells *= dim_sizes[d];
    }
    return num_cells;
}

/**
* Checks whether the game has been won.
*
* @param game Array to store the game cells.
* @param dim Number of dimensions of the game.
* @param dim_sizes Array specifying the size of each dimension of the game.
* @return 1 if the game been won, otherwise 0.
*/
int check_win(struct cell *game, int dim, int *dim_sizes) {
    // Calculate number of cells
    int num_cells = get_num_cells(dim, dim_sizes);

    // If every cell without a mine is selected, return a win.
    struct cell *c;
    for (int i=0; i<num_cells; i++) {
        c = &game[i];
        if (c->mined == 0 && c->selected == 0) {
            return 0;
        }
    }
    return 1;
}

/**
* Checks if two cells are adjacent to each other.
*
* @param dim Number of dimensions of the game.
* @param coords1 Coordinates of the first cell.
* @param coords2 Coordinates of the second cell.
* @return 1 if the cells are adjacent, otherwise 0.
*/
int is_adjacent(int dim, int *coords1, int *coords2) {
    for (int d=0; d<dim; d++) {
        int diff = abs(coords1[d] - coords2[d]);
        if (diff != 1 && diff != 0) {
            return 0;
        }
    }
    return 1;
}

/**
* Maps higher dimensional coordinates to indices of a 1D array for storage in
* the game array.
*
* @param dim Number of dimensions of the game.
* @param dim_sizes Array specifying the size of each dimension of the game.
* @param coords Coordinates of the cell.
* @return The index of the game array corresponding to the given coordinate.
*/
int coords_to_index(int dim, int *dim_sizes, int *coords) {
    // Compute a weighting factor for each dimension.
    int limit = 1;
    int dim_values[MAX_DIM];
    for (int d=dim-1; d>=0; d--) {
        dim_values[d] = limit;
        limit *= dim_sizes[d];
    }

    // Compute the sum of all the weighted coordinate values.
    int index = 0;
    for (int d=0; d<dim; d++) {
        index += dim_values[d] * coords[d];
    }

    return index;
}

/**
* Maps indices of the 1D game array to higher dimensional coordinates of the
* game state.
*
* @param coords Array to store the output coordinates from the converted index.
* @param index Index of the game array to convert to coordinates.
* @param dim Number of dimensions of the game.
* @param dim_sizes Array specifying the size of each dimension of the game.
*/
void index_to_coords(int *coords, int index, int dim, int *dim_sizes) {
    // Compute a weighting factor for each dimension.
    int limit = 1;
    int dim_values[MAX_DIM];
    for (int d=dim-1; d>=0; d--) {
        dim_values[d] = limit;
        limit *= dim_sizes[d];
    }

    // Compute the transformation from index to coordinate.
    int index_copy = index;
    for (int d=0; d<dim; d++) {
        int dim_value = dim_values[d];
        coords[d] = index_copy / dim_value;
        index_copy = index_copy % dim_value;
    }
}
