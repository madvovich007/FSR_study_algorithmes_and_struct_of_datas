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
    int vert_count = 0, edge_count = 0, source = 0, sink = 0;
    scanf("%d %d %d %d", &vert_count, &edge_count, &source, &sink);
    int ** mat = (int **)malloc(vert_count * sizeof (int*));
    int ** mat1 = (int **)malloc(vert_count * sizeof (int*));
    for (int i = 0; i < vert_count; i++){
        mat[i] = (int *) malloc(vert_count * sizeof (int));
        mat1[i] = (int *) malloc(vert_count * sizeof (int));
        for (int j = 0; j < vert_count; j++){
            mat[i][j] = 0;
            mat1[i][j] = 0;
        }
    }
    for (int i = 0; i < edge_count; i++){
        int a, b, c;
        scanf("%d %d %d", &a, &b, &c);
        mat[a][b] = c;
        mat1[a][b] = c;
    }
    int c = min_cut_ford_falkerson_alg(mat, mat1, vert_count, source, sink);
    if (c == -1){
        printf("The source and sink nodes are already disconnected");
    }
    else{
        printf("%d", c);
    }
    for (int i = 0; i < vert_count; i++){
        free(mat[i]);
        free(mat1[i]);
    }
    free(mat);
    free(mat1);
    //max_flow = min_cu
    return 0;
}
