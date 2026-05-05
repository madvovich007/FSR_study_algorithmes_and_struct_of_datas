#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define epsilon 1e-12

int get_right_down_point(double **arr, int n){
    int tmp_i = 0;
    double x_min = arr[0][0];
    double y_min = arr[0][1];
    for (int i = 0; i < n; i++) {
        if (arr[i][0] > x_min + epsilon) {
            tmp_i = i;
            x_min = arr[i][0];
            y_min = arr[i][1];
        } else {
            if (fabs(arr[i][0] - x_min) <= epsilon) {
                if (arr[i][1] < y_min - epsilon) {
                    y_min = arr[i][1];
                    tmp_i = i;
                }
            }
        }
    }
    return tmp_i;
}

int signum(double x){
    if (x > epsilon){
        return 1;
    }
    if (x < -epsilon){
        return -1;
    }
    return 0;
}

int vec_mult_sign(double *a, double *b, double *c){
    return signum((b[0] - a[0]) * (c[1] - a[1]) - (b[1] - a[1]) * (c[0] - a[0]));
}

double dist(double *a, double *b){
    double dx = a[0] - b[0];
    double dy = a[1] - b[1];
    return sqrt(dx * dx + dy * dy);
}

double **Jarvis_alg(double **arr, int n, int *hull_size){
    int *indx = (int *)malloc(n * sizeof(int));
    int count = 0;
    int start = get_right_down_point(arr, n);
    int curr = start;
    int flag = 0;

    while (flag == 0 && count < n) {
        indx[count++] = curr;
        int next = -1;
        for (int i = 0; i < n; i++) {
            if (i != curr) {
                if (next == -1) {
                    next = i;
                } else {
                    int v = vec_mult_sign(arr[curr], arr[next], arr[i]);
                    if (v < 0) {
                        next = i;
                    } else if (v == 0) {
                        if (dist(arr[curr], arr[i]) >
                            dist(arr[curr], arr[next]) + epsilon) {
                            next = i;
                        }
                    }
                }
            }
        }
        if (next == -1) {
            free(indx);
            return NULL;
        }
        curr = next;
        if (curr == start) flag = 1;
    }

    double **points = (double **)malloc(count * sizeof(double *));
    for (int i = 0; i < count; i++) {
        points[i] = (double *)malloc(2 * sizeof(double));
        points[i][0] = arr[indx[i]][0];
        points[i][1] = arr[indx[i]][1];
    }
    free(indx);
    *hull_size = count;
    return points;
}

double max_triangle_area(double **convex_hull, int hull_size){
    double min_x = convex_hull[0][0];
    double max_x = convex_hull[0][0];
    for (int i = 1; i < hull_size; i++){
        if (convex_hull[i][0] < min_x){
            min_x = convex_hull[i][0];
        }
        if (convex_hull[i][0] > max_x){
            max_x = convex_hull[i][0];
        }
    }
    double maxi_a = (-min_x < max_x) ? -min_x : max_x;
    if (maxi_a <= 0.0){
        return 0.0;
    }
    double best_S = 0.0;
    int count_of_steps = 200000;
    double tmp = maxi_a / count_of_steps;

    for (int i = 1; i <= count_of_steps; i++){
        double a = i * tmp;
        double h = 1e100;
        for (int j = 0; j < hull_size; j++){
            double x = convex_hull[j][0];
            double y = convex_hull[j][1];
            if (fabs(x) < a - epsilon){
                double val = y * a / (a - fabs(x));
                if (val < h){
                    h = val;
                }
            }
        }
        double square = a * h;
        if (square > best_S){
            best_S = square;
        }
    }
    return best_S;
}

int main(void) {
    int k;
    scanf("%d", &k);
    int total = 0;
    double **arr = NULL;
    for (int i = 0; i < k; i++){
        int n;
        scanf("%d", &n);
        arr = (double **)realloc(arr, (total + n) * sizeof(double *));
        for (int j = 0; i < n; i++) {
            arr[total + j] = (double *)malloc(2 * sizeof(double));
            scanf("%lf %lf", &arr[total + j][0], &arr[total + j][1]);
        }
        total += n;
    }


    double **uniq = (double **)malloc(total * sizeof(double *));
    int size = 0;
    for (int i = 0; i < total; i++){
        int flag = 0;
        for (int j = 0; j < size && flag == 0; j++){
            if (fabs(arr[i][0] - uniq[j][0]) < epsilon &&
                fabs(arr[i][1] - uniq[j][1]) < epsilon){
                flag = 1;
            }
        }
        if (!flag){
            uniq[size] = (double *)malloc(2 * sizeof(double));
            uniq[size][0] = arr[i][0];
            uniq[size][1] = arr[i][1];
            size++;
        }
    }


    double area = 0.0;
    if (size >= 3){
        int hull_size = 0;
        double **convex_hull = Jarvis_alg(uniq, size, &hull_size);
        if (convex_hull != NULL){
            area = max_triangle_area(convex_hull, hull_size);
            for (int i = 0; i < hull_size; i++) free(convex_hull[i]);
            free(convex_hull);
        }
    }
    printf("%.6lf", area);
    for (int i = 0; i < total; i++){
        free(arr[i]);
    }
    free(arr);
    for (int i = 0; i < size; i++){
        free(uniq[i]);
    }
    free(uniq);
    return 0;
}