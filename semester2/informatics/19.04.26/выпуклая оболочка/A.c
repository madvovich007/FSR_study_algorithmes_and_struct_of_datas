#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define infinity 1e7
#define epsilon 1e-12




int signum(double x){
    if (x > epsilon){
        return 1;
    }
    else{
        if (x< -epsilon){
            return -1;
        }
    }
    return 0;
}

int vec_mult_sign(double* a, double* b, double *c){
    return signum((b[0] - a[0]) * (c[1] - a[1]) - (b[1] - a[1]) * (c[0] - a[0]));
}


double dist(double * a, double* b){
    return sqrt((a[0] - b[0])* (a[0] - b[0]) + (a[1] - b[1])* (a[1] - b[1]));
}


int main(void){
    int n = 0;
    int x_p, y_p;
    scanf("%d %d %d", &n, &x_p, &y_p);
    int * x = (int*)malloc(n * sizeof (int));
    int * y = (int*)malloc(n * sizeof (int));
    for (int i = 0; i < n; i++){
        scanf("%d %d", &x[i], &y[i]);
    }
    int counter = 0;
    for (int i = 0; i < n; i++){
        int x1 = x[i];
        int x2 = x[(i + 1)%n];
        int y1 = y[i];
        int y2 = y[(i + 1)%n];
        double a[2] = {x1, y1};
        double b[2] = {x2, y2};
        double p[2] = {x_p, y_p};
        if (vec_mult_sign(a, b, p) == 0){
            printf("YES");
            free(x);
            free(y);
            return 0;
        }
        if ((y1 > y_p) != (y2 > y_p)){
            double tmp_x = x1 + (double)(x2 - x1) * (y_p - y1) / (y2 - y1);
            if (tmp_x > x_p){
                counter++;
            }
        }
    }

    if (counter % 2 == 1){
        printf("YES");
    }
    else{
        printf("NO");
    }
    free(x);
    free(y);
    return 0;
}