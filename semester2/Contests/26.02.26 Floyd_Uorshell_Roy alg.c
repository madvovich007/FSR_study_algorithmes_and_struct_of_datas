#include <stdio.h>
#include <stdlib.h>

#define infinity 1e7

void floyd_uorshell_roy_alg(int n, double **mat, double **d){
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            if (i == j){
                d[i][j] = 0.0;
            }
            else{
                if (mat[i][j] > 0){
                    d[i][j] = mat[i][j];
                }
                else{
                    d[i][j] = infinity;
                }
            }
        }
    }

    for (int k = 0; k < n; k++){
        for (int i = 0; i < n; i++){
            for (int j = 0; j < n; j++){
                if (d[i][k] + d[k][j] < d[i][j]){
                    d[i][j] = d[i][k] + d[k][j];
                }
            }
        }
    }
    return;
}

int main(void){
    int n = 0, k = 0, m = 0;
    scanf("%d %d %d", &n, &k, &m);
    double ** mat = (double **) malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++){
        mat[i] = (double *) malloc(n * sizeof(double));
        for (int j = 0; j < n; j++){
            scanf("%lf", &mat[i][j]);
        }
    }

    double ** d = (double **) malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++){
        d[i] = (double *) malloc(n * sizeof(double));
    }

    double min_d = infinity;
    int tmp_i = -1, tmp_j = -1;
    for (int i = 0; i < k; i++){
        for (int j = n - m; j < n; j++){
            if (d[i][j] < min_d){
                min_d = d[i][j];
                tmp_i = i;
                tmp_j = j;
            }
        }
    }

    printf("%d %d", tmp_i, tmp_j);

    for (int i = 0; i < n; i++){
        free(mat[i]);
        free(d[i]);
    }
    free(mat);
    free(d);
    return 0;
}
