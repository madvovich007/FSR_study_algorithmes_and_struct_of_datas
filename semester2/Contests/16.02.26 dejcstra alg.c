#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define infinity 1e7

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

double dijkstra_alg(int v1, int v2, int n, double ** mat){
    int * visited = (int *)calloc(n,  sizeof (int));
    double * d = (double *)calloc(n, sizeof (double));
    for (int i = 0; i < n; i++){
        d[i] = infinity;
    }
    d[v1] = 0;
    int flag = 0;
    while (full_of_visit(visited, n) == 0 && flag == 0){
        double min_dist = infinity;
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
    double c = d[v2];
    free(visited);
    free(d);
    if (c == infinity){
        c= -1;
    }
    return c;
}


int main(void){
    int v1 = 0, v2 = 0, n = 0;
    scanf("%d %d %d", &v1, &v2, &n);
    double ** mat = (double ** ) malloc(n * sizeof (double *));
    for (int i = 0; i < n; i++){
        mat[i] = (double*) malloc(n * sizeof (double));
    }
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            scanf("%lf", &mat[i][j]);
        }
    }
    v1--;
    v2--;
    double c = dijkstra_alg(v1, v2, n, mat);
    if (c == -1){
        printf("%d", -1);
    }
    else {
        printf("%.0lf", c);
    }
    for (int i = 0; i < n; i++){
        free(mat[i]);
    }
    free(mat);
    return 0;
}