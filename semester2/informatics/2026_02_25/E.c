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


typedef struct answer{
    int i;
    int j;
    int d;
} answer;


double kruskals_alg(edge * edges, int n_vert, int n_edges, DSU ** dsu, int * count_of_added_edges, answer ans[], int m){
    if (n_vert <= 1){
        return 0.0;
    }
    quick_sort(edges, n_edges);
    *count_of_added_edges = 0;
    double total_sum = 0;
    int edge_used = 0;

    int count_of_comp = 0;
    for (int i = 0; i < n_vert; i++){
        if (find_set(dsu[i]) == dsu[i]){
            count_of_comp++;
        }
    }

    for (int i = 0; i < n_edges && edge_used < count_of_comp - 1; i++){
        edge tmp_e = edges[i];
        DSU * h_u = find_set((dsu[tmp_e.u]));
        DSU * h_v = find_set((dsu[tmp_e.v]));
        if (h_u != h_v){
            union_set(h_u, h_v);
            ans[*count_of_added_edges].i = (edges[i].u) / m;// cтрока
            ans[*count_of_added_edges].j = (edges[i].u) % m;// cтолбец
            int dist = 0;
            if (abs(edges[i].u - edges[i].v) == m){
                dist = 1;
            }
            else{
                dist = 2;
            }
            ans[*count_of_added_edges].d = dist;//дистанция
            (*count_of_added_edges)++;
            total_sum += edges[i].weight;
            edge_used++;
        }
    }
    return total_sum;
}


int main(void){
    int n = 0, m = 0;
    scanf("%d %d", &n, &m);
    DSU ** dsu = (DSU**) malloc(n * m * sizeof (DSU*));
    for (int i = 0; i < n * m; i++){
        dsu[i] = make_set();
    }
    for (int i = 0; i < n; i++){
        for (int j = 0; j < m; j++){
            int value_of_connection = 0;
            scanf("%d", &value_of_connection);
            switch (value_of_connection){
                case 1:
                    if (i < n - 1) {
                        union_set(dsu[i * m + j], dsu[(i + 1) * m + j]);
                    }
                    break;
                case 2:
                    if (j < m - 1) {
                        union_set(dsu[i * m + j], dsu[i * m + j + 1]);
                    }
                    break;
                case 3:
                    if (i < n - 1) {
                        union_set(dsu[i * m + j], dsu[(i + 1) * m + j]);
                    }
                    if (j < m - 1) {
                        union_set(dsu[i * m + j], dsu[i * m + j + 1]);
                    }
                    break;
                default:
                    break;
            }
        }
    }

    int count_of_full_edge = (n - 1) * m + n * (m - 1);
    edge * edges = (edge *) malloc(count_of_full_edge * sizeof (edge));
    int count = 0;

    for (int i = 0; i < n - 1; i++){
        for (int j = 0; j < m; j ++){
            edges[count].u = i * m + j;
            edges[count].v = (i + 1) * m + j;
            edges[count].weight = 1.0;
            count++;
        }
    }

    for (int i = 0; i < n; i++){
        for (int j = 0; j < m - 1; j ++){
            edges[count].u = i * m + j;
            edges[count].v = i * m + j + 1;
            edges[count].weight = 2.0;
            count++;
        }
    }
    answer *ans = (answer *)malloc(count_of_full_edge * sizeof (answer));
    int count_of_added_edges = 0;
    double total_cost = kruskals_alg(edges, n * m, count_of_full_edge, dsu, &count_of_added_edges, ans, m);

    printf("%d %d\n", count_of_added_edges, (int)total_cost);
    for (int i = 0; i < count_of_added_edges; i++){
        printf("%d %d %d\n", ans[i].i + 1, ans[i].j + 1, ans[i].d);
    }
    for (int i= 0; i < n * m; i++){
        free(dsu[i]);
    }
    free(dsu);
    free(edges);
    free(ans);
    return 0;
}