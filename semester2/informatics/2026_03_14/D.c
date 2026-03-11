#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define infinity 10000000
#define epsilon 1e-12
#define truck_mass 3000000
#define maxi_time 1440//tk 1 day = 24 hours = 1440 minutes

int full_of_visit(int * visited, int n){
    int flag = 0;
    for (int i = 0; i < n && flag == 0; i++){
        if (visited[i] != 1){
            flag = 1;
        }
    }
    if (flag == 1){
        return 0;
    }
    return 1;
}

int  dijkstra_alg(int v1, int v2, int n, int ** mat){
    int * visited = (int *)calloc(n,  sizeof (int));
    int * d = (int *)calloc(n, sizeof (int));
    for (int i = 0; i < n; i++){
        d[i] = infinity;
    }
    d[v1] = 0;
    int flag = 0;
    while (full_of_visit(visited, n) == 0 && flag == 0){
        int min_dist = infinity;
        int tmp_v = -1;
        for (int i = 0; i < n; i++){
            if (d[i] < min_dist && visited[i] == 0){
                tmp_v = i;
                min_dist = d[i];
            }
        }
        if (tmp_v == -1){
            flag = 1;
        }
        if (tmp_v != -1) {
            visited[tmp_v] = 1;
            for (int i = 0; i < n; i++) {
                if (mat[tmp_v][i] >= 0 && visited[i] == 0) {
                    if (d[i] > d[tmp_v] + mat[tmp_v][i]) {
                        d[i] = d[tmp_v] + mat[tmp_v][i];
                    }
                }
            }
        }
    }
    int c = d[v2];
    free(visited);
    free(d);
    if (c == infinity){
        c= -1;
    }
    return c;
}


typedef struct edge{
    int u;
    int v;
    int time;
    int weight;
} edge;


int check_cups_possibility(int x, int n, int m, edge * edges){
    int ** mat = (int **) malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++){
        mat[i] = (int *) malloc(n * sizeof(int));
        for (int j = 0; j < n; j++){
            mat[i][j] = -1;
        }
    }

    for (int i = 0; i < m; i++){
        if (edges[i].weight - truck_mass >= 100 * x) {
            mat[edges[i].u][edges[i].v] = edges[i].time;
            mat[edges[i].v][edges[i].u] = edges[i].time;
        }
    }
    int total_time = dijkstra_alg(0, n - 1, n, mat);
    for (int i = 0; i < n; i++){
        free(mat[i]);
    }
    free(mat);
    if (total_time != -1 && total_time <= maxi_time){
        return 1;
    }
    return 0;
}

int main(void){
    int  n = 0, m = 0;
    scanf("%d %d", &n, &m);
    edge * edges = (edge *) malloc(m * sizeof (edge));
    int maxi = 0;
    for (int i = 0; i < m ; i++){
        edge tmp;
        scanf("%d %d %d %d", &tmp.u, &tmp.v, &tmp.time, &tmp.weight);
        tmp.u--;
        tmp.v--;
        edges[i] = tmp;
        if (tmp.weight - truck_mass >= 0) {
            int capacity = ((tmp.weight - truck_mass) / 100);
            if (capacity > maxi) {
                maxi = capacity;
            }
        }
    }
    int down_limit = 0, high_linit = maxi;
    int total = 0;
    while (down_limit <= high_linit){
        int tmp = (down_limit + high_linit) / 2;
        if (check_cups_possibility(tmp, n, m, edges)){
            total = tmp;
            down_limit = tmp + 1;
        }
        else{
            high_linit = tmp - 1;
        }
    }
    if (n == 1){
        printf("%d", infinity);
    }
    else {
        printf("%d", total);
    }
    free(edges);
}