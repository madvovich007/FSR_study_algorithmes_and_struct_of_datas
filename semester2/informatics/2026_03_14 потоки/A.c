#include <stdio.h>
#include <stdlib.h>

#define infinity 1e7

typedef struct point{
    int x;
    int y;
}hummock;

void adding(int x, int y, hummock ** points, int * count){
    for (int i = 0; i < *count; ++i){
        if ((*points)[i].x == x && (*points)[i].y == y){
            return;
        }
    }
    *points = (hummock *)realloc(*points, (*count + 1) * sizeof (hummock));
    (*points)[*count].x = x;
    (*points)[*count].y = y;
    (*count)++;
}


int bfs_find_path(int** mat, int n, int beg, int end, int * prev){
    int * queue = (int *)malloc(n * sizeof (int));
    int head = 0;
    int tail = 0;
    for (int i = 0; i < n; i++){
        prev[i] = -1;
    }
    queue[tail] = beg;
    tail++;
    while(head < tail){
        int tmp = queue[head++];
        for (int i = 0; i < n; i++){
            if (mat[tmp][i] > 0 && prev[i] == -1){
                prev[i] = tmp;
                if (i == end){
                    free(queue);
                    return 1;
                }
                queue[tail++] = i;
            }
        }
    }
    free(queue);
    return 0;
}
int min_cut_ford_falkerson_alg(int ** mat, int ** mat1, int vert_count, int source, int sink){
    int maxi_flow = 0;
    int * prev = (int *)malloc(vert_count * sizeof (int));
    while (bfs_find_path(mat1, vert_count, source, sink, prev)){
        int tmp_flow = infinity;
        int v = sink;
        while (v != source){
            int u = prev[v];
            if (mat1[u][v] < tmp_flow){
                tmp_flow = mat1[u][v];
            }
            v = u;
        }
        v = sink;
        while (v != source){
            int u = prev[v];
            mat1[u][v] -= tmp_flow;
            mat1[v][u] += tmp_flow;
            v = u;
        }
        maxi_flow += tmp_flow;
    }
    free(prev);
    if (maxi_flow <= 0){
        return -1;
    }
    return maxi_flow;
}


int main(void){
    int n = 0;
    scanf("%d", &n);
    int ** mat = (int **) malloc(n * sizeof (int*));
    for (int i = 0; i < n ; i++){
        mat[i] = (int *) malloc(4 * sizeof (int));
        scanf("%d %d %d %d", &mat[i][0], &mat[i][1], &mat[i][2], &mat[i][3]);
    }
    int start_x = 0;
    int start_y = 0;
    scanf("%d %d", &start_x, &start_y);
    int finish_x = 0;
    int finish_y = 0;
    scanf("%d %d", &finish_x, &finish_y);

    hummock * points = NULL;
    int counter = 0;
    for (int i = 0; i < n; i++){
        adding(mat[i][0], mat[i][1], &points, &counter);
        adding(mat[i][2], mat[i][3], &points, &counter);
    }
    adding(start_x, start_y, &points, &counter);
    adding(finish_x, finish_y, &points, &counter);

    int starting_index = -1, finishing_index = -1;
    for (int i = 0; i < counter; i++){
        if (points[i].x == start_x && points[i].y == start_y){
            starting_index = i;
        }
        if (points[i].x == finish_x && points[i].y == finish_y){
            finishing_index = i;
        }
    }
    int * in_index = (int * )malloc(counter * sizeof (int));
    int * out_index = (int * )malloc(counter * sizeof (int));
    for (int i = 0; i < counter; i++){
        in_index[i] = -1;
        out_index[i] = -1;
    }

    int next = 0;
    in_index[starting_index] = next;
    out_index[starting_index] = next;
    next++;
    in_index[finishing_index] = next;
    out_index[finishing_index] = next;
    next++;
    for (int i = 0; i < counter; i++){
        if (i != starting_index && i!= finishing_index){
            in_index[i] = next;
            out_index[i] = next + 1;
            next++;
            next++;
        }
    }

    int **mat1 = (int ** ) malloc(next * sizeof (int *));
    for (int i = 0; i < next; i++){
        mat1[i] = (int *) calloc(next , sizeof (int));
    }
    for (int i = 0; i < counter; i++){
        if (i != starting_index && i != finishing_index){
            mat1[in_index[i]][out_index[i]] = 1;
        }
    }
    for (int i = 0; i < n; i++){
        int u = -1;
        int v = -1;
        for (int j = 0; j < counter; j++){
            if (points[j].x == mat[i][0] && points[j].y == mat[i][1]){
                u = j;
            }
            if (points[j].x == mat[i][2] && points[j].y == mat[i][3]){
                v = j;
            }
        }
        if (u == starting_index){
            mat1[in_index[u]][in_index[v]] = infinity;
        }
        else{
            if (v == starting_index){
                mat1[in_index[v]][in_index[u]] = infinity;
            }
            else{
                if (u == finishing_index){
                    mat1[out_index[v]][in_index[u]] = infinity;
                }
                else{
                    if (v == finishing_index){
                        mat1[out_index[u]][in_index[v]] = infinity;
                    }
                    else{
                        mat1[out_index[u]][in_index[v]] = infinity;
                        mat1[out_index[v]][in_index[u]] = infinity;
                    }
                }
            }
        }
    }
    int total = min_cut_ford_falkerson_alg(mat1, mat1, next, in_index[starting_index], in_index[finishing_index]);
    if (total == -1){
        total = 0;
    }
    printf("%d", total);
    for (int i = 0; i < next; i++){
        free(mat1[i]);
    }
    for (int i = 0; i < n; i++){
        free(mat[i]);
    }
    free(in_index);
    free(out_index);
    free(points);
    free(mat);
    free(mat1);
    //max_flow = min_cut
    return 0;
}
