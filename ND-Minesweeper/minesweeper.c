#include "minesweeper.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int count_num = 0;

void cartesianProduct(int **coor_cell, int **arr_dim, int *d_size,
    int *current, int dim, int times) {

    	if(times == dim){
            for(int i = 0; i < dim; i++){
                coor_cell[count_num][i] = current[i];
            }
            count_num++;

    	}else{
    		for(int j = 0; j < d_size[times]; j++){
    			current[times] = arr_dim[times][j];
                cartesianProduct(coor_cell, arr_dim, d_size, current, dim
                    , times+1);
    		}
    	}
}

//called first and exactly once
void init_game(struct cell * game, int dim, int * dim_sizes, int num_mines,
    int ** mined_cells) {

    //coordinates of n-dimensions
    int *current = (int*)calloc(sizeof(int),dim);
    int **arr_dim = (int **)malloc(dim*sizeof(int *));
    for(int j = 0; j < dim; j++) {
        arr_dim[j] = (int *)malloc(dim_sizes[j]*sizeof(int));
    }
    for(int i = 0; i < dim; i++) {
        for(int j = 0; j < dim_sizes[i]; j++){
            arr_dim[i][j] = j;
        }
    }
    //e.g
    // 0123
    // 0123456789
    // 012345

    // array of coordinates
    int num_cell = 1;
    for(int j = 0; j < dim; j++){
        num_cell = num_cell*dim_sizes[j];
    }
    int **coor_cell = (int **)malloc(num_cell*sizeof(int *));
    for(int j = 0; j < num_cell; j++) {
        coor_cell[j] = (int *)malloc(dim*sizeof(int));
    }
    //
    // set coordinates
    cartesianProduct(coor_cell, arr_dim, dim_sizes, current, dim, 0);
    //

    //assign coordinates to cell and initialising
    for(int i = 0; i < num_cell; i++) {
        for(int j = 0; j < dim; j++) {
            game[i].coords[j] = coor_cell[i][j];
            game[i].selected = 0;
            game[i].num_adjacent = 0;
            game[i].mined = 0;
            game[i].hint = 0;
        }
    }

    //count adjacent, set mine and set adjacent_cells
    for(int i = 0; i < num_cell; i++) {
        int adj_count = 0;
        for(int j = 0; j < num_cell; j++) {
            int max = INT_MIN;
            for(int k = 0; k < dim; k++) {
                int diff = abs(game[i].coords[k] - game[j].coords[k]);
                if(diff > max){
                    max = diff;
                }
            }

            if(max == 1) {
                game[i].adjacent[adj_count] = &game[j];
                adj_count++;
            }
        }
        game[i].num_adjacent = adj_count;
    }

    // set mined
    for(int j = 0; j < num_mines; j++) {
        for(int i = 0; i < num_cell; i++) {
            int match = 1;
            for(int k = 0; k < dim; k++) {
                if(game[i].coords[k] != mined_cells[j][k]) {
                    match = 0;
                    break;
                }
            }
            if(match == 1) {
                game[i].mined = 1;
            }
        }
    }

    //set hint
    for(int i = 0; i < num_cell; i++) {
        for(int j = 0; j < game[i].num_adjacent; j++) {
            if(game[i].adjacent[j]->mined == 1){
                game[i].hint++;
            }
        }
    }

    // free pointer
    for(int j = 0; j < num_cell; j++) {
        free(coor_cell[j]);
    }
    for(int j = 0; j < dim; j++) {
        free(arr_dim[j]);
    }
    free(arr_dim);
    free(coor_cell);
    free(current);

    return;
}

void select_recur(struct cell *c) {
    if(c->hint == 0) {
        for(int i = 0; i < c->num_adjacent; i++) {
            if(c->adjacent[i]->selected == 0 && c->adjacent[i]->mined != 1) {
                c->adjacent[i]->selected = 1;
                select_recur(c->adjacent[i]);
            }
        }
    }
}

int select_cell(struct cell * game, int dim, int * dim_sizes, int * coords) {
    int num_cell = 1;
    for(int j = 0; j < dim; j++){
        num_cell = num_cell*dim_sizes[j];
    }

    //select cells
    int count_mined = 0;
    for(int i = 0; i < num_cell; i++) {
        int match = 1;
        for(int j = 0; j < dim; j++){
            if(game[i].coords[j] != coords[j]) {
                match = 0;
                break;
            }
        }

        // after match
        if(match == 1) {
            // select a cell with a mine
            if(game[i].mined == 1) {
                game[i].selected = 1;
                return 1;
            }else{
                // select a cell without mine
                game[i].selected = 1;
                select_recur(&game[i]);
            }
        }

        //count mined cells
        if(game[i].mined == 1) {
            count_mined++;
        }
    }

    //check win
    int count_slt_epty = 0;
    for(int i = 0; i < num_cell; i++) {
        if(game[i].mined == 0 && game[i].selected == 1) {
            count_slt_epty++;
        }
    }
    int not_mined = num_cell - count_mined;
    if(count_slt_epty == not_mined) {
        return 2;
    }


    return 0;
}

// for testing, comment before uploading
// int main(){
//
//     //initialising
//     int dim = 2;
//
//     int dim_sizes[] = {2, 4};
//
//     int num_cell = 1;
//     for(int i = 0; i < dim; i++){
//         num_cell *= dim_sizes[i];
//     }
//
//     struct cell game[100];
//     for(int i = 0; i < num_cell; i++){
//         struct cell c = {0, 0, 0, 0, 0, 0};
//         game[i] = c;
//     }
//
//     int num_mine_cell = 1;
//     int **mine_cell = (int **)malloc(num_mine_cell*sizeof(int *));
//     for (int i=0; i<num_mine_cell; i++){
//         mine_cell[i] = (int *)malloc(dim*sizeof(int));
//     }
//     for(int i = 0; i < num_mine_cell; i++) {
//         for(int j = 0; j < dim; j++) {
//             mine_cell[i][j] = 1;
//         }
//     }
//
//     for(int i = 0; i < num_mine_cell; i++) {
//         for(int j = 0; j < dim; j++) {
//             printf("%d ", mine_cell[i][j]);
//         }
//         printf("\n");
//     }
//
//     int num_mine = 1;
//     //
//
//     init_game(game, dim, dim_sizes, num_mine, mine_cell);
//
//     //struct cell * game, int dim, int * dim_sizes, int * coords
//     int coords[] = {1, 1};
//     int r = select_cell(game, dim, dim_sizes, coords);
//     printf("return: %d\n", r);
//
//     printf("-----------------------------------------\n");
//     for(int i = 0; i < num_cell; i++) {
//         printf("coords: ");
//         for(int j = 0; j < dim; j++){
//             printf("%d", game[i].coords[j]);
//         }
//         printf("\n");
//         printf("mined: %d\n", game[i].mined);
//         printf("selected %d\n", game[i].selected);
//         printf("num_adjacent %d\n", game[i].num_adjacent);
//         printf("hint %d\n", game[i].hint);
//         printf("----------------------------------\n");
//     }
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//     return 0;
// }
