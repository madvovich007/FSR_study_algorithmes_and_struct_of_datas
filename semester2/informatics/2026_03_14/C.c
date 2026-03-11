#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define infinity 1e18
#define maxi_n 20


typedef struct pos{
    int x;
    int y;
    int direction;
    int count_of_turns;
}position;


int bfs(int k, int m, int n, int field[20][20], int start_x, int start_y, int finish_x, int finish_y){
    int direct_x[4] = {-1, 0, 1,0};
    int direct_y[4] = {0, 1,0, -1};

    int arr[n][m][4][k + 1];
    for (int i = 0; i < n; i++){
        for (int j = 0; j < m; j++){
            for (int o = 0; o < 4; o++){
                for (int p = 0; p < k + 1; p++){
                    arr[i][j][o][p] = -1;
                }
            }
        }
    }

    position * queue = (position *)malloc(n * m * 4 * (k + 1) *sizeof (position));
    int head = 0;
    int tail = 0;
    for (int i = 0; i < 4; i++){
        arr[start_x][start_y][i][0] = 0;
        position tmp;
        tmp.x = start_x;
        tmp.y = start_y;
        tmp.direction = i;
        tmp.count_of_turns = 0;
        queue[tail++] = tmp;
    }

    while (head < tail){
        position curr = queue[head++];
        int curr_d = arr[curr.x][curr.y][curr.direction][curr.count_of_turns];

        // left
        int tmp_dir = (curr.direction + 3) % 4;
        int tmp_turn = curr.count_of_turns;
        int tmp_x = curr.x + direct_x[tmp_dir];
        int tmp_y = curr.y + direct_y[tmp_dir];
        if (tmp_x >= 0 && tmp_x < n && tmp_y >= 0 && tmp_y < m && field[tmp_x][tmp_y] != 1){
            if (arr[tmp_x][tmp_y][tmp_dir][tmp_turn] == -1){
                arr[tmp_x][tmp_y][tmp_dir][tmp_turn] = curr_d + 1;
                position tmp;
                tmp.x = tmp_x;
                tmp.y = tmp_y;
                tmp.direction = tmp_dir;
                tmp.count_of_turns = tmp_turn;
                queue[tail++] = tmp;
            }
        }

        //forward
        tmp_dir = curr.direction;
        tmp_turn = curr.count_of_turns;
        tmp_x = curr.x + direct_x[tmp_dir];
        tmp_y = curr.y + direct_y[tmp_dir];
        if (tmp_x >= 0 && tmp_x < n && tmp_y >= 0 && tmp_y < m  && field[tmp_x][tmp_y] != 1){
            if (arr[tmp_x][tmp_y][tmp_dir][tmp_turn] == -1){
                arr[tmp_x][tmp_y][tmp_dir][tmp_turn] = curr_d + 1;
                position tmp;
                tmp.x = tmp_x;
                tmp.y = tmp_y;
                tmp.direction = tmp_dir;
                tmp.count_of_turns = tmp_turn;
                queue[tail++] = tmp;
            }
        }

        //right
        if (curr.count_of_turns < k) {
            tmp_dir = (curr.direction + 1) % 4;
            tmp_turn = curr.count_of_turns + 1;
            tmp_x = curr.x + direct_x[tmp_dir];
            tmp_y = curr.y + direct_y[tmp_dir];
            if (tmp_x >= 0 && tmp_x < n && tmp_y >= 0 && tmp_y < m  && field[tmp_x][tmp_y] != 1){
                if (arr[tmp_x][tmp_y][tmp_dir][tmp_turn] == -1){
                    arr[tmp_x][tmp_y][tmp_dir][tmp_turn] = curr_d + 1;
                    position tmp;
                    tmp.x = tmp_x;
                    tmp.y = tmp_y;
                    tmp.direction = tmp_dir;
                    tmp.count_of_turns = tmp_turn;
                    queue[tail++] = tmp;
                }
            }
        }
    }
    int total = -1;
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < k + 1; j++){
            if (arr[finish_x][finish_y][i][j] != -1){
                if (total == -1 || arr[finish_x][finish_y][i][j] < total){
                    total = arr[finish_x][finish_y][i][j];
                }
            }
        }
    }
    free(queue);
    return total;
}


int main(void){
    int k = 0; int m = 0; int n = 0;
    scanf("%d %d %d", &k, &n, &m);
    int arr[maxi_n][maxi_n];
    int start_x = -1, start_y = -1, finish_x = -1, finish_y = -1;
    for (int i = 0; i < n; i++){
        for (int j = 0; j < m; j++){
            scanf("%d", &arr[i][j]);
            if (arr[i][j] == 2){
                start_x = i;
                start_y = j;
            }
            else{
                if (arr[i][j] == 3){
                    finish_x = i;
                    finish_y = j;
                }
            }
        }
    }
    printf("%d", bfs(k, m ,n , arr, start_x, start_y, finish_x, finish_y));
    return 0;
}