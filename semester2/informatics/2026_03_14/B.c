#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define infinity 1e18


typedef struct edges{
    int start;
    int finish;
    long long dep;
    long long arr;
}edge;


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

long long dijkstra_alg(int v1, int v2, int n, edge * edges, int edge_counter){
    int * visited = (int *)calloc(n,  sizeof (int));
    long long * d = (long long *)calloc(n, sizeof (long long));
    for (int i = 0; i < n; i++){
        d[i] = infinity;
    }
    d[v1] = 0;
    int flag = 0;
    while (full_of_visit(visited, n) == 0 && flag == 0){
        long long min_dist = infinity;
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
            for (int i = 0; i < edge_counter; i++) {
                if (edges[i].start == tmp_v && edges[i].dep >= d[tmp_v]) {
                    if (edges[i].arr < d[edges[i].finish]) {
                        d[edges[i].finish] = edges[i].arr;
                    }
                }
            }
        }
    }
    long long c = d[v2];
    free(visited);
    free(d);
    if (c >= infinity){
        c= -1;
    }
    return c;
}


int main(void){
    int v1 = 0, v2 = 0, n = 0;
    scanf("%d %d", &n, &v2);
    v2--;
    int m = 0;
    scanf("%d", &m);

    edge * edges = (edge *) malloc(n * sizeof (edge));
    int edge_counter = 0;
    int edge_capacity = n;

    for (int i = 0; i < m; i++){
        int count_of_vert = 0;
        scanf("%d", &count_of_vert);
        long long prev_time = 0;
        int prev_tmp_vert = -1;

        for (int j = 0; j < count_of_vert; j++){
            long long time = 0;
            int tmp_vert = 0;
            scanf("%d %lld", &tmp_vert, &time);
            tmp_vert--;
            if (j != 0){
                if (edge_counter >= edge_capacity){
                    edge_capacity *= 2;
                    edges = (edge*)realloc(edges, edge_capacity * sizeof (edge));
                }
                edges[edge_counter].start = prev_tmp_vert;
                edges[edge_counter].finish = tmp_vert;
                edges[edge_counter].dep = prev_time;
                edges[edge_counter].arr = time;
                edge_counter++;
            }
            prev_time = time;
            prev_tmp_vert = tmp_vert;
        }
    }
    printf("%lld", dijkstra_alg(v1, v2, n, edges, edge_counter));

    free(edges);
}