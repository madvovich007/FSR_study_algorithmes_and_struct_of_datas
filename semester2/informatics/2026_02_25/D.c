#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct edge{
    int u;
    int v;
    double weight;
    int index;
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


typedef struct mst_edge{
    int len_of_connection;
    int index_of_connection;
} mst_edge;


double kruskals_alg(edge * edges, int n_vert, int n_edges, int * total_len, mst_edge * mst){
    if (n_vert <= 1){
        *total_len = 0;
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
            mst[edge_used].index_of_connection = tmp_e.index;
            mst[edge_used].len_of_connection = (int)tmp_e.weight;
            total_sum+= tmp_e.weight;
            union_set(h_u, h_v);
            edge_used++;
        }
    }

    for (int i = 0; i < n_vert; i++){
        free(dsu[i]);
    }
    free(dsu);
    *total_len = total_sum;
    return edge_used;
}


int main(void){
    int n = 0, m = 0;
    scanf("%d %d", &n, &m);
    edge * edges = (edge *)malloc(m * sizeof (edge));
    for (int i = 0; i < m; i++){
        int x, y, z;
        scanf("%d %d %d", &x, &y, &z);
        edges[i].u = x - 1;
        edges[i].v = y - 1;
        edges[i].weight = z;
        edges[i].index = i + 1;
    }

    int p5, q5, p6, q6;
    scanf("%d %d %d %d", &p5, &q5, &p6, &q6);

    mst_edge * mst = (mst_edge * )malloc((n - 1) * sizeof (mst_edge));
    int total_len = 0;
    int sum_count_mst = kruskals_alg(edges, n, m, &total_len, mst);

    if (sum_count_mst != n - 1 || total_len > q5 + q6){
        printf("Impossible\n");
        free(mst);
        free(edges);
        return 0;
    }
    //поимаем, какой из кабелей дороже
    int cheap_cost, expensive_cost, cheap_lenght, expensive_lenght;
    int flag_of_first = 0;//если орогой первый - то 1.
    if (p5 <= p6){
        cheap_cost = p5;
        cheap_lenght = q5;
        expensive_cost = p6;
        expensive_lenght = q6;
        flag_of_first = 1;
    }
    else{
        cheap_cost = p6;
        cheap_lenght = q6;
        expensive_cost = p5;
        expensive_lenght = q5;
    }

    //дешевый кабель покрывает от (общее - длина дорогого) до (длина дешевого)
    double down_limit = total_len - expensive_lenght;
    double up_limit = cheap_lenght;
    if (down_limit > up_limit){
        printf("Impossible\n");
        free(mst);
        free(edges);
        return 0;
    }

    int * last = (int *)malloc(((int)up_limit + 1) * sizeof (int));
    for (int i = 1; i <= (int)up_limit; i++){
        last[i] = -1;
    }
    last[0] = -2;//чтобы отличить, что 0 достигается из любой точки, -1 - пока не дошагали

    for (int i = 0; i < total_len; i++){
        int len = mst[i].len_of_connection;
        for (int j = (int)up_limit; j >= len; j--){
            if (last[j - len] != -1 && last[j] == -1){
                last[j] = i;
            }
        }
    }

    int targ = -1;
    int flag = 0;
    for (int i = (int)up_limit; i >= down_limit && flag == 1; i--){
        if (last[i] != -1){
            targ = i;
        }
    }

    // восстанавливаем путь

    char * cheap_edges = (char *)malloc(total_len * sizeof(char));
    for (int i = 0; i < total_len; i++){
        cheap_edges[i] = 0;
    }
    int tmp = targ;
    while (tmp > 0){
        int index = last[tmp];
        cheap_edges[index] = 1;
        tmp = tmp - mst[index].len_of_connection;
    }

    printf("%d\n", (cheap_cost * targ + expensive_cost * (total_len - targ)));

    for (int i = 0; i < total_len; i++){
        int category = 0;
        if (flag_of_first){
            if (cheap_edges[i]){
                category = 5;
            }
            else{
                category = 6;
            }
        }
        else{
            if (cheap_edges[i]){
                category = 6;
            }
            else{
                category = 5;
            }
        }
        printf("%d %d\n", mst[i].index_of_connection, category);
    }
    return 0;
}
