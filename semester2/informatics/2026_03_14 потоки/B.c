#include <stdio.h>
#include <stdlib.h>

#define infinity 1e7


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
int min_cut_ford_falkerson_alg(int ** mat1, int vert_count, int source, int sink){
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
    int *types_of_cables = (int*)malloc(n * sizeof (int));
    for (int i = 0; i < n; i++){
        scanf("%d", &types_of_cables[i]);
    }
    int ** mat = (int**)malloc(n * sizeof (int*));
    for (int i = 0; i < n; i++){
        mat[i] = (int *)malloc(n * sizeof(int));
        for (int j = 0; j < n; j++){
            scanf("%d", &mat[i][j]);
        }
    }

    int new_n = n + 2;
    int ** g = (int**)malloc(new_n * sizeof (int*));
    for (int i = 0; i < new_n; i++){
        g[i] = (int *)calloc(new_n , sizeof(int));
    }
    int sink = n + 1;
    int source = n;

    for (int i = 0; i < n ; i++){
        for (int j = 0; j < n ; j++){
            if (mat[i][j] != 0){
                g[i][j] = mat[i][j];
            }
        }
    }
    for (int i = 0; i < n; i++){
        if (types_of_cables[i] == 1){
            g[source][i] = infinity;
        }
        if (types_of_cables[i] == 2){
            g[i][sink] = infinity;
        }
    }
    printf("%d", min_cut_ford_falkerson_alg(g, new_n, source, sink));
    for (int i = 0; i < n; i++){
        free(mat[i]);
    }
    for (int i = 0; i < new_n; i++){
        free(g[i]);
    }
    free(mat);
    free(g);
    free(types_of_cables);
    //max_flow = min_cut
    return 0;
}