#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct edge{
    int u;
    int v;
    double weight;
} edge;


typedef struct DSU{
    struct DSU * parent;
    int rang;
} DSU;

DSU* make_set(){
    DSU * tmp = (DSU *) malloc(sizeof (DSU));
    tmp->parent = tmp;
    tmp->rang = 1;
    return tmp;
}

DSU * find_set(DSU * node){
    if (node->parent == node){
        return node;
    }
    else{
        node->parent = find_set(node->parent);
        return node->parent;
    }
}


void union_set(DSU * a, DSU * b){
    a = find_set(a);
    b = find_set(b);
    if (a == b){
        return;
    }
    if (a->rang < b ->rang){
        DSU * tmp = a;
        a = b;
        b = tmp;
    }
    b->parent = a;
    if (a->rang == b->rang){
        a->rang++;
    }
    return;
}


void quick_sort(edge arr[], int n){

    if (n <= 1){
        return;
    }
    double sup = arr[n / 2].weight;
    int i = 0;
    int j = n - 1;
    if (n > 1){
        while (i <= j){
            while (arr[i].weight < sup){
                i++;
            }
            while (arr[j].weight > sup){
                j--;
            }
            if (i <= j){
                edge tmp = arr[i];
                arr[i] = arr[j];
                arr[j] = tmp;
                i++;
                j--;
            }
        }
        quick_sort(arr, j + 1);
        quick_sort(arr + i, n - i);
    }
}



double kruskals_alg(edge * edges, int n_vert, int n_edges){
    if (n_vert <= 1){
        return 0.0;
    }
    quick_sort(edges, n_edges);

    DSU ** dsu= (DSU **)malloc(n_vert * sizeof (DSU *));
    for (int i = 0; i < n_vert; i++){
        dsu[i] = make_set();
    }

    double total_sum = 0;
    int edge_used = 0;
    for (int i = 0; i < n_edges && edge_used < n_vert - 1; i++){
        edge tmp_e = edges[i];
        DSU * h_u = find_set((dsu[tmp_e.u]));
        DSU * h_v = find_set((dsu[tmp_e.v]));
        if (h_u != h_v){
            total_sum+= tmp_e.weight;
            union_set(h_u, h_v);
            edge_used++;
        }
    }

    for (int i = 0; i < n_vert; i++){
        free(dsu[i]);
    }
    free(dsu);
    return total_sum;
}




int main(void){
    int n = 0;
    scanf("%d", &n);
    double * x_coord = (double *) malloc(n * sizeof (double));
    double * y_coord = (double *) malloc(n * sizeof (double));
    for (int i = 0; i < n; i++){
        scanf("%lf %lf", &x_coord[i], &y_coord[i]);
    }

    int count_of_full_edge = n * (n - 1) / 2;
    edge * edges = (edge*) malloc((count_of_full_edge * sizeof (edge)));
    int count = 0;
    for (int i = 0; i < n; i++){
        for (int j = i + 1; j < n; j++){
            double dist = sqrt((x_coord[i] - x_coord[j]) * (x_coord[i] - x_coord[j]) + (y_coord[i] - y_coord[j]) * (y_coord[i] - y_coord[j]));
            edges[count].u = i;
            edges[count].v = j;
            edges[count].weight = dist;
            count++;
        }
    }
    int m = 0;
    scanf("%d", &m);
    for (int i = 0; i < m; i++){
        int x = 0, y = 0;
        scanf("%d %d", &x, &y);
        x--;
        y--;
        if (x > y){
            int tmp = x;
            x = y;
            y = tmp;
        }
        for (int j = 0; j < count_of_full_edge; j++){
            if (edges[j].u == x && edges[j].v == y){
                edges[j].weight = 0.0;
            }
        }
    }
    printf("%.5lf", kruskals_alg(edges, n, n * (n - 1) / 2));

    free(edges);
    free(x_coord);
    free(y_coord);
    return 0;
}