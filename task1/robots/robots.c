#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BOARD_SIZE (8)
#define BUFF_LEN (100)
#define ROBOT_NUM (90)

typedef struct coordinate{
    int x;
    int y;
}coordinate;
int move_robot(int robot_num, int hum_x, int hum_y, coordinate robot_arr[], char map[BOARD_SIZE][BOARD_SIZE], char direc[], int exit_x, int exit_y){
    for(int i = 0; i < BOARD_SIZE; i++){
        for(int j = 0; j < BOARD_SIZE; j++){
            if(map[i][j] == 'R'){
                map[i][j] = ' ';
            }
        }
    }

    for(int i = 0; i < robot_num; i++){
        if(robot_arr[i].y == hum_y && robot_arr[i].x < hum_x){
            //check if destory
            if(map[robot_arr[i].y][robot_arr[i].x] != 'D'){
                robot_arr[i].y += 1;
            }
        }else if(robot_arr[i].y == hum_y && robot_arr[i].x > hum_x){
            if(map[robot_arr[i].y][robot_arr[i].x] != 'D'){
                robot_arr[i].y -= 1;
            }
        }else if(robot_arr[i].x == hum_x && robot_arr[i].y < hum_y){
            if(map[robot_arr[i].y][robot_arr[i].x] != 'D'){
                robot_arr[i].x += 1;
            }
        }else if(robot_arr[i].x == hum_x && robot_arr[i].y > hum_y){
            if(map[robot_arr[i].y][robot_arr[i].x] != 'D'){
                robot_arr[i].x -= 1;
            }
        }else if(robot_arr[i].x != hum_x && robot_arr[i].y != hum_y){
            if(strcmp(direc, "NT") == 0 || strcmp(direc, "ST") == 0){
                if(map[robot_arr[i].y][robot_arr[i].x] != 'D'){
                    if(robot_arr[i].x < hum_x){
                        robot_arr[i].x += 1;
                    }else if(robot_arr[i].x > hum_x){
                        robot_arr[i].x -= 1;
                    }
                }
            }else if(strcmp(direc, "WT") == 0 || strcmp(direc, "ET") == 0){
                if(map[robot_arr[i].y][robot_arr[i].x] != 'D'){
                    if(robot_arr[i].y < hum_y){
                        robot_arr[i].y += 1;
                    }else if(robot_arr[i].y > hum_y){
                        robot_arr[i].y -= 1;
                    }
                }
            }
        }
    }

    for(int j = 0; j < robot_num; j++){
        //check if capture human
        if(robot_arr[j].x == hum_x && robot_arr[j].y == hum_y){
            printf("%s\n", "The robots got you! We are doomed!");

            return 1;
        }
        // check if destroy
        for(int i = 0; i < robot_num; i++){
            if(robot_arr[j].x == robot_arr[i].x && robot_arr[j].y == robot_arr[i].y){
                map[robot_arr[j].y][robot_arr[j].x] = 'D';
            }
        }
    }

    for(int l = 0; l < robot_num; l++){
        map[robot_arr[l].y][robot_arr[l].x] = 'R';
    }

    if(map[exit_y][exit_x] != 'R'){
        map[exit_y][exit_x] = 'E';
    }

    return 0;


}

void print_board(char map[BOARD_SIZE][BOARD_SIZE]){
    printf("\n");
    for(int i = 0; i < BOARD_SIZE; i++){
        for(int j = 0; j < BOARD_SIZE; j++){
            printf("|");
            printf("%c", map[i][j]);
        }
        printf("|");
        printf("\n");
    }
    printf("\n");
}



int main(int argc, char** argv){
    if(argc < 2){
        return 0;
    }
     int restart = 1;
     while( restart == 1){
        char buff[BUFF_LEN];

        char map[BOARD_SIZE][BOARD_SIZE];
        //char board_pos[BOARD_SIZE][BOARD_SIZE];
        memset(map, ' ', BOARD_SIZE*BOARD_SIZE);
        //memset(board_pos, ' ', BOARD_SIZE*BOARD_SIZE);

        int hum_x;
        int hum_y;

        int exit_x;
        int exit_y;

        int robot_num;

        //char* arr[2] = {"Human starting point: ", "Number of robots: "};

        printf("Human starting point: ");
        while(fgets(buff, BUFF_LEN, stdin)){
            if(sscanf(buff, "%d %d", &hum_x, &hum_y) == 2){
                if(hum_x >= 0 && hum_x < 8 && hum_y >= 0 && hum_y < 8){
                    map[hum_y][hum_x] = 'H';
                    srand(atoi(argv[1]));
                    exit_x = rand() % 8;
                    exit_y = rand() % 8;

                    map[exit_y][exit_x] = 'E';
                    break;
                }else{
                    printf("Invalid Coordinates, Try again\n");
                    printf("Human starting point: ");
                }
            }else{
                printf("Invalid Coordinates, Try again\n");
                printf("Human starting point: ");
            }
        }

        printf("Number of robots: ");
        while(fgets(buff, BUFF_LEN, stdin)){
            if(sscanf(buff, "%d", &robot_num) == 1){
                if(robot_num > 0){
                    break;
                }else{
                    printf("Invalid number, Try again\n");
                    printf("Number of robots: ");
                }
            }else{
                printf("Invalid number, Try again\n");
                printf("Number of robots: ");
            }
        }

        int robot_x;
        int robot_y;

        coordinate robot_arr[ROBOT_NUM];

        for(int i = 1; i < robot_num+1; i++){
            printf("Robot %d: ", i);
            while(fgets(buff, BUFF_LEN, stdin)){
                if(sscanf(buff, "%d %d", &robot_x, &robot_y) == 2){
                    if((abs(robot_x - hum_x) >= 2 || abs(robot_y - hum_y) >= 2) && robot_x < 8 && robot_y < 8 && robot_x >=0 && robot_y >= 0){
                        if(map[robot_y][robot_x] == 'R'){
                            printf("Invalid Coordinates, Try again\n");
                            printf("Robot %d: ", i);
                        }else{
                            coordinate robot = {robot_x, robot_y};
                            robot_arr[i-1] = robot;
                            map[robot_y][robot_x] = 'R';
                            break;
                        }
                    }else{
                        printf("Invalid Coordinates, Try again\n");
                        printf("Robot %d: ", i);
                    }
                }else{
                    printf("Invalid Coordinates, Try again\n");
                    printf("Robot %d: ", i);
                }
            }
        }
        printf("\n");


        char move[4];
        char direc[2];
        char display[7];

        while(fgets(buff, BUFF_LEN, stdin)){
            if(sscanf(buff, "%s %s", move, direc) == 2){
                if(strcmp(direc, "NT") == 0){

                    printf("\n");
                    map[hum_y][hum_x] = ' ';
                    hum_y -= 1;
                    if(map[hum_y][hum_x] == 'R'){
                        printf("%s\n\n", "The robots got you! We are doomed!");
                        restart = 1;
                        break;
                    }else if(map[hum_y][hum_x] == 'E'){
                        printf("%s\n", "Well done! You survived, but it isn't over!");
                        restart = 1;
                        break;
                    }else{
                        //move human
                        map[hum_y][hum_x] = 'H';
                        //move robots
                        if(move_robot(robot_num, hum_x, hum_y, robot_arr, map, direc, exit_x, exit_y) == 1){
                            restart = 1;
                            break;
                        }
                    }

                }else if(strcmp(direc, "ST") == 0){

                    printf("\n");
                    map[hum_y][hum_x] = ' ';
                    hum_y += 1;
                    if(map[hum_y][hum_x] == 'R'){
                        printf("%s\n\n", "The robots got you! We are doomed!");
                        restart = 1;
                        break;
                    }else if(map[hum_y][hum_x] == 'E'){
                        printf("%s\n", "Well done! You survived, but it isn't over!");
                        restart = 1;
                        break;
                    }else{
                        //move human
                        map[hum_y][hum_x] = 'H';
                        //move robots
                        if(move_robot(robot_num, hum_x, hum_y, robot_arr, map, direc, exit_x, exit_y) == 1){
                            restart = 1;
                            break;
                        }
                    }

                }else if(strcmp(direc, "WT") == 0){

                    printf("\n");
                    map[hum_y][hum_x] = ' ';
                    hum_x -= 1;
                    if(map[hum_y][hum_x] == 'R'){
                        printf("%s\n\n", "The robots got you! We are doomed!");
                        restart = 1;
                        break;
                    }else if(map[hum_y][hum_x] == 'E'){
                        printf("%s\n", "Well done! You survived, but it isn't over!");
                        restart = 1;
                        break;
                    }else{
                        //move human
                        map[hum_y][hum_x] = 'H';
                        //move robots
                        if(move_robot(robot_num, hum_x, hum_y, robot_arr, map, direc, exit_x, exit_y) == 1){
                            restart = 1;
                            break;
                        }
                    }

                }else if(strcmp(direc, "ET") == 0){

                    printf("\n");
                    map[hum_y][hum_x] = ' ';
                    hum_x += 1;
                    if(map[hum_y][hum_x] == 'R'){
                        printf("%s\n\n", "The robots got you! We are doomed!");
                        restart = 1;
                        break;
                    }else if(map[hum_y][hum_x] == 'E'){
                        printf("%s\n", "Well done! You survived, but it isn't over!");
                        restart = 1;
                        break;
                    }else{
                        //move human
                        map[hum_y][hum_x] = 'H';
                        //move robots
                        if(move_robot(robot_num, hum_x, hum_y, robot_arr, map, direc, exit_x, exit_y) == 1){
                            restart = 1;
                            break;
                        }
                    }

                }else{
                    printf("%s\n", "Invalid Coordinates, Try again");
                }
            }else if(sscanf(buff, "%s", display) == 1){
                if(strcmp(display, "display") == 0){
                    print_board(map);
                }else if(strcmp(display, "exit") == 0){
                    restart = 0;
                    break;
                }else{
                    printf("Invalid command1\n");
                }
            }else{
                printf("Invalid command2\n");
            }
        }
    }


    return 0;
}
