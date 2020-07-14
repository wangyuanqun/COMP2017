#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFF_LEN (100)
#define BOARD_SIZE (8)
typedef struct{
    int x;
    int y;
}coordinate;

void print_board(char board[BOARD_SIZE][BOARD_SIZE]){
    //print first line in board
    printf(" ");
    for(int i = 1; i < BOARD_SIZE+1; i++){
        printf(" ");
        printf("%d", i);
    }
    printf("\n");
    //print board
    char arr[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', '\0'};
    for(int i = 0; i < BOARD_SIZE; i++){
        printf("%c", arr[i]);
        for(int j = 0; j < BOARD_SIZE; j++){
            printf("|");
            printf("%c", board[i][j]);
        }
        printf("|");
        printf("\n");
    }
    printf("\n");

}

coordinate* set_pos(coordinate arr[], int size, char a, char h, int v, char vh){
    int ver = v - 1;
    int hori = h - 65;
    if(a == 'C'){
        if(vh == 'H'){
            for(int i = 0; i < size; i++){
                coordinate co = {hori, ver};
                arr[i] = co;
                ver++;
            }
        }else{
            for(int i = 0; i < size; i++){
                coordinate co = {hori, ver};
                arr[i] = co;
                hori++;
            }
        }
        return arr;
    }else if(a == 'B'){
        if(vh == 'H'){
            for(int i = 0; i < size; i++){
                coordinate co = {hori, ver};
                arr[i] = co;
                ver++;
            }
        }else{
            for(int i = 0; i < size; i++){
                coordinate co = {hori, ver};
                arr[i] = co;
                hori++;
            }
        }
        return arr;
    }else if(a == 'D'){
        if(vh == 'H'){
            for(int i = 0; i < size; i++){
                coordinate co = {hori, ver};
                arr[i] = co;
                ver++;
            }
        }else{
            for(int i = 0; i < size; i++){
                coordinate co = {hori, ver};
                arr[i] = co;
                hori++;
            }
        }
        return arr;
    }else if(a == 'S'){
        if(vh == 'H'){
            for(int i = 0; i < size; i++){
                coordinate co = {hori, ver};
                arr[i] = co;
                ver++;
            }
        }else{
            for(int i = 0; i < size; i++){
                coordinate co = {hori, ver};
                arr[i] = co;
                hori++;
            }
        }
        return arr;
    }else if(a == 'P'){
        if(vh == 'H'){
            for(int i = 0; i < size; i++){
                coordinate co = {hori, ver};
                arr[i] = co;
                ver++;
            }
        }else{
            for(int i = 0; i < size; i++){
                coordinate co = {hori, ver};
                arr[i] = co;
                hori++;
            }
        }
        return arr;
    }

    return 0;
}

int collide(char a[BOARD_SIZE][BOARD_SIZE], coordinate b[], int s_b, char v_or_h, int v_po, char h_po){
    int collide = 0;


    if(s_b == 5){
        if(v_or_h == 'H' && v_po > 4){
            collide = 1;
        }else if(v_or_h == 'V' && h_po > 'D'){
            collide = 1;
        }else{
            for(int i = 0; i < s_b; i++){
                if(a[b[i].x][b[i].y] == 'C' || a[b[i].x][b[i].y] == 'B' || a[b[i].x][b[i].y] == 'D' || a[b[i].x][b[i].y] == 'S' || a[b[i].x][b[i].y] == 'P'){
                    collide = 1;
                    break;
                }
            }
        }
    }else if(s_b == 4){
        if(v_or_h == 'H' && v_po > 5){
            collide = 1;
        }else if(v_or_h == 'V' && h_po > 'E'){
            collide = 1;
        }else{
            for(int i = 0; i < s_b; i++){
                if(a[b[i].x][b[i].y] == 'C' || a[b[i].x][b[i].y] == 'B' || a[b[i].x][b[i].y] == 'D' || a[b[i].x][b[i].y] == 'S' || a[b[i].x][b[i].y] == 'P'){
                    collide = 1;
                    break;
                }
            }
        }
    }else if(s_b == 3){
        if(v_or_h == 'H' && v_po > 6){
            collide = 1;
        }else if(v_or_h == 'V' && h_po > 'F'){
            collide = 1;
        }else{
            for(int i = 0; i < s_b; i++){
                if(a[b[i].x][b[i].y] == 'C' || a[b[i].x][b[i].y] == 'B' || a[b[i].x][b[i].y] == 'D' || a[b[i].x][b[i].y] == 'S' || a[b[i].x][b[i].y] == 'P'){
                    collide = 1;
                    break;
                }
            }
        }
    }else if(s_b == 2){
        if(v_or_h == 'H' && v_po > 7){
            collide = 1;
        }else if(v_or_h == 'V' && h_po > 'G'){
            collide = 1;
        }else{
            for(int i = 0; i < s_b; i++){
                if(a[b[i].x][b[i].y] == 'C' || a[b[i].x][b[i].y] == 'B' || a[b[i].x][b[i].y] == 'D' || a[b[i].x][b[i].y] == 'S' || a[b[i].x][b[i].y] == 'P'){
                    collide = 1;
                    break;
                }
            }
        }
    }else if(s_b == 1){
        if(v_or_h == 'H' && v_po > 7){
            collide = 1;
        }else if(v_or_h == 'V' && h_po > 'G'){
            collide = 1;
        }else{
            for(int i = 0; i < 2; i++){
                if(a[b[i].x][b[i].y] == 'C' || a[b[i].x][b[i].y] == 'B' || a[b[i].x][b[i].y] == 'D' || a[b[i].x][b[i].y] == 'S' || a[b[i].x][b[i].y] == 'P'){
                    collide = 1;
                    break;
                }
            }
        }
    }


    return collide;

}




int main(){
    int ship_len = 5;
    char buff[BUFF_LEN];
    //player1 hit_board and board
    char board_player1[BOARD_SIZE][BOARD_SIZE];
    char board_player1_ship[BOARD_SIZE][BOARD_SIZE];
    //player2 hit_board and board
    char board_player2[BOARD_SIZE][BOARD_SIZE];
    char board_player2_ship[BOARD_SIZE][BOARD_SIZE];

    char h_po;
    int v_po;
    char v_or_h;

    memset(board_player1, ' ', BOARD_SIZE*BOARD_SIZE);
    memset(board_player2, ' ', BOARD_SIZE*BOARD_SIZE);
    memset(board_player1_ship, ' ', BOARD_SIZE*BOARD_SIZE);
    memset(board_player2_ship, ' ', BOARD_SIZE*BOARD_SIZE);

    // print_board(board_player1);
    // print_board(board_player2);

    char* arr[5] = {"Carrier: ", "Battleship: ", "Destroyer: ", "Submarine: ", "Patrol Boat: "};

    printf("%s", "Player 1, please set up your ships (y, x, orientation)\n");
    for(int i = 0; i < 5; i++){
        printf("%s", arr[i]);
        while (fgets(buff, BUFF_LEN, stdin)){
            if(sscanf(buff, "%c %d %c", &h_po, &v_po, &v_or_h) == 3){
                if(h_po < 'A' || h_po > 'H' || v_po < 1 || v_po > 8 ||(v_or_h != 'H' && v_or_h != 'V')){
                    printf("%s\n", "Invalid ship configuration");
                    printf("%s", arr[i]);
                }else if(ship_len == 5){
                    coordinate co[5];
                    coordinate* coo = set_pos(co, ship_len, arr[i][0], h_po, v_po, v_or_h);
                    if(collide(board_player1_ship, coo, ship_len, v_or_h, v_po, h_po) == 1){
                        printf("%s\n", "Invalid ship configuration");
                        printf("%s", arr[i]);
                    }else{
                        for(int j = 0; j < ship_len; j++){
                            board_player1_ship[coo[j].x][coo[j].y] = 'C';
                        }
                        ship_len--;
                        break;
                    }

                }else if(ship_len == 4){
                    coordinate co[4];
                    coordinate* coo = set_pos(co, ship_len, arr[i][0], h_po, v_po, v_or_h);
                    if(collide(board_player1_ship, coo, ship_len, v_or_h, v_po, h_po) == 1){
                        printf("%s\n", "Invalid ship configuration");
                        printf("%s", arr[i]);
                    }else{
                        for(int j = 0; j < ship_len; j++){
                            board_player1_ship[coo[j].x][coo[j].y] = 'B';
                        }
                        ship_len--;
                        break;
                    }
                }else if(ship_len == 3){
                    coordinate co[3];
                    coordinate* coo = set_pos(co, ship_len, arr[i][0], h_po, v_po, v_or_h);
                    if(collide(board_player1_ship, coo, ship_len, v_or_h, v_po, h_po) == 1){
                        printf("%s\n", "Invalid ship configuration");
                        printf("%s", arr[i]);
                    }else{
                        for(int j = 0; j < ship_len; j++){
                            board_player1_ship[coo[j].x][coo[j].y] = 'D';
                        }
                        ship_len--;
                        break;
                    }

                }else if(ship_len == 2){
                    coordinate co[2];
                    coordinate* coo = set_pos(co, ship_len, arr[i][0], h_po, v_po, v_or_h);
                    if(collide(board_player1_ship, coo, ship_len, v_or_h, v_po, h_po) == 1){
                        printf("%s\n", "Invalid ship configuration");
                        printf("%s", arr[i]);
                    }else{
                        for(int j = 0; j < ship_len; j++){
                            board_player1_ship[coo[j].x][coo[j].y] = 'S';
                        }
                        ship_len--;
                        break;
                    }

                }else if(ship_len == 1){
                    coordinate co[2];
                    coordinate* coo = set_pos(co, 2, arr[i][0], h_po, v_po, v_or_h);
                    if(collide(board_player1_ship, coo, ship_len, v_or_h, v_po, h_po) == 1){
                        printf("%s\n", "Invalid ship configuration");
                        printf("%s", arr[i]);
                    }else{
                        for(int j = 0; j < 2; j++){
                            board_player1_ship[coo[j].x][coo[j].y] = 'P';
                        }
                        break;
                    }

                }
            }else{
                printf("Unrecognised command\n");
                printf("%s", arr[i]);
            }
        }
    }
    ship_len = 5;


    printf("\n");
    printf("%s", "\nPlayer 2, please set up your ships (y, x, orientation)\n");
    for(int j = 0; j < 5; j++){
        printf("%s", arr[j]);
        while (fgets(buff, BUFF_LEN, stdin)){
            if(sscanf(buff, "%c %d %c", &h_po, &v_po, &v_or_h) == 3){
                if(h_po < 'A' || h_po > 'H' || v_po < 1 || v_po > 8 ||(v_or_h != 'H' && v_or_h != 'V')){
                    printf("%s\n", "Invalid ship configuration");
                    printf("%s", arr[j]);
                }else if(ship_len == 5){
                    coordinate co[5];
                    coordinate* coo = set_pos(co, ship_len, arr[j][0], h_po, v_po, v_or_h);
                    if(collide(board_player2_ship, coo, ship_len, v_or_h, v_po, h_po) == 1){
                        printf("%s\n", "Invalid ship configuration");
                        printf("%s", arr[j]);
                    }else{
                        for(int l = 0; l < ship_len; l++){
                            board_player2_ship[coo[l].x][coo[l].y] = 'C';
                        }
                        ship_len--;
                        break;
                    }

                }else if(ship_len == 4){
                    coordinate co[4];
                    coordinate* coo = set_pos(co, ship_len, arr[j][0], h_po, v_po, v_or_h);
                    if(collide(board_player2_ship, coo, ship_len, v_or_h, v_po, h_po) == 1){
                        printf("%s\n", "Invalid ship configuration");
                        printf("%s", arr[j]);
                    }else{
                        for(int l = 0; l < ship_len; l++){
                            board_player2_ship[coo[l].x][coo[l].y] = 'B';
                        }
                        ship_len--;
                        break;
                    }
                }else if(ship_len == 3){
                    coordinate co[3];
                    coordinate* coo = set_pos(co, ship_len, arr[j][0], h_po, v_po, v_or_h);
                    if(collide(board_player2_ship, coo, ship_len, v_or_h, v_po, h_po) == 1){
                        printf("%s\n", "Invalid ship configuration");
                        printf("%s", arr[j]);
                    }else{
                        for(int l = 0; l < ship_len; l++){
                            board_player2_ship[coo[l].x][coo[l].y] = 'D';
                        }
                        ship_len--;
                        break;
                    }

                }else if(ship_len == 2){
                    coordinate co[2];
                    coordinate* coo = set_pos(co, ship_len, arr[j][0], h_po, v_po, v_or_h);
                    if(collide(board_player2_ship, coo, ship_len, v_or_h, v_po, h_po) == 1){
                        printf("%s\n", "Invalid ship configuration");
                        printf("%s", arr[j]);
                    }else{
                        for(int l = 0; l < ship_len; l++){
                            board_player2_ship[coo[l].x][coo[l].y] = 'S';
                        }
                        ship_len--;
                        break;
                    }

                }else if(ship_len == 1){
                    coordinate co[2];
                    coordinate* coo = set_pos(co, 2, arr[j][0], h_po, v_po, v_or_h);
                    if(collide(board_player2_ship, coo, ship_len, v_or_h, v_po, h_po) == 1){
                        printf("%s\n", "Invalid ship configuration");
                        printf("%s", arr[j]);
                    }else{
                        for(int l = 0; l < 2; l++){
                            board_player2_ship[coo[l].x][coo[l].y] = 'P';
                        }
                        ship_len--;
                        break;
                    }

                }
            }else{
                printf("Unrecognised command\n");
                printf("%s", arr[j]);
            }
        }
    }

    printf("%s\n", "\nAll boats set up, Player 1 can now strike.");
    printf("\n");

    //all done above!!!!!!!!!!!!!

    // get commands after setting

    int C = 5;
    int B = 4;
    int D = 3;
    int S = 2;
    int P = 2;

    int C_2 = 5;
    int B_2 = 4;
    int D_2 = 3;
    int S_2 = 2;
    int P_2 = 2;

    char command[10];
    char vr;
    int col;

    unsigned int turn = 0;

    while (fgets(buff, BUFF_LEN, stdin)) {

        if(sscanf(buff, "%s %c %d", command, &vr, &col) == 3){
            int ver=vr-65;
            col-=1;
            if(vr < 'A' || vr > 'H' || col < 0 || col > 7){
                //printf("\n");
                printf("%s\n", "Invalid coordinates");
            }else{
                if(strcmp(command, "fire") == 0){

                    if(turn%2 == 1){
                        if(board_player1_ship[ver][col] == ' '){
                            printf("\n%s\n\n", "You have missed!");
                            board_player1[ver][col] = '0';
                            board_player1_ship[ver][col] = '0';
                            turn++;
                            printf("%s\n\n", "Player 1's turn");
                        }else if(board_player1_ship[ver][col] == 'C'){
                            board_player1[ver][col] = 'X';
                            board_player1_ship[ver][col] = 'X';
                            C--;
                            if(C == 0){
                                printf("\n%s\n\n", "We have sunk your Carrier!");
                            }else{
                                printf("\n%s\n\n", "We have hit the target!");
                            }
                            if(C==0 && B==0 && D==0 && S==0 && P==0){
                                printf("%s\n", "Player 2 wins!");
                                return 0;
                            }else{
                                printf("%s\n\n", "Player 1's turn");
                                turn++;
                            }

                        }else if(board_player1_ship[ver][col] == 'B'){

                            board_player1[ver][col] = 'X';
                            board_player1_ship[ver][col] = 'X';
                            B--;
                            if(B == 0){
                                printf("\n%s\n\n", "We have sunk your Battleship!");
                            }else{
                                printf("\n%s\n\n", "We have hit the target!");
                            }
                            if(C==0 && B==0 && D==0 && S==0 && P==0){
                                printf("%s\n", "Player 2 wins!");
                                return 0;
                            }else{
                                printf("%s\n\n", "Player 1's turn");
                                turn++;
                            }


                        }else if(board_player1_ship[ver][col] == 'D'){

                            board_player1[ver][col] = 'X';
                            board_player1_ship[ver][col] = 'X';
                            D--;
                            if(D == 0){
                                printf("\n%s\n\n", "We have sunk your Destroyer!");
                            }else{
                                printf("\n%s\n\n", "We have hit the target!");
                            }
                            if(C==0 && B==0 && D==0 && S==0 && P==0){
                                printf("%s\n", "Player 2 wins!");
                                return 0;
                            }else{
                                printf("%s\n\n", "Player 1's turn");
                                turn++;
                            }

                        }else if(board_player1_ship[ver][col] == 'S'){

                            board_player1[ver][col] = 'X';
                            board_player1_ship[ver][col] = 'X';
                            S--;
                            if(S == 0){
                                printf("\n%s\n\n", "We have sunk your Submarine!");
                            }else{
                                printf("\n%s\n\n", "We have hit the target!");
                            }
                            if(C==0 && B==0 && D==0 && S==0 && P==0){
                                printf("%s\n", "Player 2 wins!");
                                return 0;
                            }else{
                                printf("%s\n\n", "Player 1's turn");
                                turn++;
                            }

                        }else if(board_player1_ship[ver][col] == 'P'){

                            board_player1[ver][col] = 'X';
                            board_player1_ship[ver][col] = 'X';
                            P--;
                            if(P == 0){
                                printf("\n%s\n\n", "We have sunk your Patrol Boat!");
                            }else{
                                printf("\n%s\n\n", "We have hit the target!");
                            }
                            if(C==0 && B==0 && D==0 && S==0 && P==0){
                                printf("%s\n", "Player 2 wins!");
                                return 0;
                            }else{
                                printf("%s\n\n", "Player 1's turn");
                                turn++;
                            }

                        }else if(board_player1_ship[ver][col] == 'X' || board_player1_ship[ver][col] == '0'){
                            printf("%s\n", "You have already fired at this location");
                            //turn++;
                        }
                    }else if(turn%2 == 0){
                        if(board_player2_ship[ver][col] == ' '){
                            printf("\n%s\n\n", "You have missed!");
                            board_player2[ver][col] = '0';
                            board_player2_ship[ver][col] = '0';
                            turn++;
                            printf("%s\n\n", "Player 2's turn");
                        }else if(board_player2_ship[ver][col] == 'C'){

                            board_player2[ver][col] = 'X';
                            board_player2_ship[ver][col] = 'X';
                            C_2--;
                            if(C_2 == 0){
                                printf("\n%s\n\n", "We have sunk your Carrier!");
                            }else{
                                printf("\n%s\n\n", "We have hit the target!");
                            }
                            if(C_2==0 && B_2==0 && D_2==0 && S_2==0 && P_2==0){
                                printf("%s\n", "Player 1 wins!");
                                return 0;
                            }else{
                                printf("%s\n\n", "Player 2's turn");
                                turn++;
                            }

                        }else if(board_player2_ship[ver][col] == 'B'){

                            board_player2[ver][col] = 'X';
                            board_player2_ship[ver][col] = 'X';
                            B_2--;
                            if(B_2 == 0){
                                printf("\n%s\n\n", "We have sunk your Battleship!");
                            }else{
                                printf("\n%s\n\n", "We have hit the target!");
                            }
                            if(C_2==0 && B_2==0 && D_2==0 && S_2==0 && P_2==0){
                                printf("%s\n", "Player 1 wins!");
                                return 0;
                            }else{
                                printf("%s\n\n", "Player 2's turn");
                                turn++;
                            }


                        }else if(board_player2_ship[ver][col] == 'D'){

                            board_player2[ver][col] = 'X';
                            board_player2_ship[ver][col] = 'X';
                            D_2--;
                            if(D_2 == 0){
                                printf("\n%s\n\n", "We have sunk your Destroyer!");
                            }else{
                                printf("\n%s\n\n", "We have hit the target!");
                            }
                            if(C_2==0 && B_2==0 && D_2==0 && S_2==0 && P_2==0){
                                printf("%s\n", "Player 1 wins!");
                                return 0;
                            }else{
                                printf("%s\n\n", "Player 2's turn");
                                turn++;
                            }

                        }else if(board_player2_ship[ver][col] == 'S'){

                            board_player2[ver][col] = 'X';
                            board_player2_ship[ver][col] = 'X';
                            S_2--;
                            if(S_2 == 0){
                                printf("\n%s\n\n", "We have sunk your Submarine!");
                            }else{
                                printf("\n%s\n\n", "We have hit the target!");
                            }
                            if(C_2==0 && B_2==0 && D_2==0 && S_2==0 && P_2==0){
                                printf("%s\n", "Player 1 wins!");
                                return 0;
                            }else{
                                printf("%s\n\n", "Player 2's turn");
                                turn++;
                            }

                        }else if(board_player2_ship[ver][col] == 'P'){

                            board_player2[ver][col] = 'X';
                            board_player2_ship[ver][col] = 'X';
                            P_2--;
                            if(P_2 == 0){
                                printf("\n%s\n\n", "We have sunk your Patrol Boat!");
                            }else{
                                printf("\n%s\n\n", "We have hit the target!");
                            }
                            if(C_2==0 && B_2==0 && D_2==0 && S_2==0 && P_2==0){
                                printf("%s\n", "Player 1 wins!");
                                return 0;
                            }else{
                                printf("%s\n\n", "Player 2's turn");
                                turn++;
                            }

                        }else if(board_player2_ship[ver][col] == 'X' || board_player2_ship[ver][col] == '0'){
                            printf("%s\n", "You have already fired at this location");
                            //turn++;
                        }
                    }


                }else{
                    printf("%s\n", "Unrecognised command");
                }

            }
        }else if(sscanf(buff, "%s", command) == 1){
            if(strcmp(command, "shots") == 0){
                printf("\n");
                if(turn%2 == 0){
                    print_board(board_player2);
                    //printf("%s\n", "Player 2's turn");
                }else if(turn%2 == 1){
                    print_board(board_player1);
                    //printf("%s\n", "Player 1's turn");
                }
            }else if(strcmp(command, "exit") == 0){
                    return 0;
            }else{
                printf("%s\n", "Unrecognised command");
            }
        }
    }

    return 0;
}
