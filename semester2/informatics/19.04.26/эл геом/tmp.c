#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define epsilon 1e-10


double max(double a, double b){
    if (a > b){
        return a;
    }
    return b;
}

double min(double a, double b){
    if (a < b){
        return a;
    }
    return b;
}


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

int vec_mult_sign(double x1, double y1, double x2, double y2, double x3, double y3){
    return signum((x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1));
}


double dist(int x1, int y1, int x2, int y2){
    return sqrt((x1 - x2) * (x1- x2) + (y1 - y2) * (y1 - y2));
}


int on_seg (int x1, int y1, int x2, int y2, int x3, int y3){
    return (x3 >= min(x1, x2) &&
            x3 <= max(x1, x2) &&
            y3 >= min(y1, y2) &&
            y3 <= max(y1, y2));
}

void check_cross(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4, int i, int j, int *counter, int *ans){
    int a = vec_mult_sign(x1, y1, x2, y2, x3, y3);
    int b = vec_mult_sign(x1, y1, x2, y2, x4, y4);
    int c = vec_mult_sign(x3, y3, x4, y4, x1, y1);
    int d = vec_mult_sign(x3, y3, x4, y4, x2, y2);
    if (a != b && c != d){
        if (i < j){
            ans[*counter * 2] = i;
            ans[*counter * 2 + 1] = j;
        }
        else{
            ans[*counter * 2] = j;
            ans[*counter * 2 + 1] = i;
        }
        (*counter)++;
        return;
    }
    if ((a == 0 && on_seg(x1, y1, x2, y2, x3, y3)) ||
        (b == 0 && on_seg(x1, y1, x2, y2, x4, y4)) ||
        (c == 0 && on_seg(x3, y3, x4, y4, x1, y1)) ||
        (d == 0 && on_seg(x3, y3, x4, y4, x2, y2))){
        if (i < j){
            ans[*counter * 2] = i;
            ans[*counter * 2 + 1] = j;
        }
        else{
            ans[*counter * 2] = j;
            ans[*counter * 2 + 1] = i;
        }
        (*counter)++;
        return;
    }
    return;
}

int main(void){
    int n = 0;
    scanf("%d", &n);
    int * indx = (int*)malloc(n * sizeof(int));
    double *x1 = (double *)malloc(n * sizeof(double));
    double *y1 = (double *)malloc(n * sizeof(double));
    double *x2 = (double *)malloc(n * sizeof(double));
    double *y2 = (double *)malloc(n * sizeof(double));

    for (int i = 0; i < n; i++){
        scanf("%d %lf %lf %lf %lf", &indx[i], &x1[i], &y1[i], &x2[i], &y2[i]);
    }
    int counter = 0;
    int* ans = (int*)malloc(n * (n - 1) / 2 * sizeof(int));
    for (int i = 0; i < n; i++){
        for (int j = i + 1; j < n; j++){
            check_cross(x1[i], y1[i], x2[i], y2[i], x1[j], y1[j], x2[j], y2[j], indx[i], indx[j], &counter, ans);
        }
    }

    for (int i = 0; i < counter - 1; i++){
        for (int j = 0; j < counter - i - 1; j++){
            int a1 = ans[j * 2], a2 = ans[j * 2 + 1];
            int b1 = ans[(j + 1) * 2], b2 = ans[(j + 1) * 2 + 1];
            if (a1 > b1 || (a1 == b1 && a2 > b2)){
                ans[j * 2] = b1;
                ans[j * 2 + 1] = b2;
                ans[(j + 1) * 2] = a1;
                ans[(j + 1) * 2 + 1] = a2;
            }
        }
    }

    for (int i = 0; i < counter; i++){
        printf("%d %d\n", ans[i * 2], ans[i * 2 + 1]);
    }

    free(indx);
    free(x1);
    free(y1);
    free(x2);
    free(y2);

    free(ans);
    return 0;
}