#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define infinity 1e7
#define epsilon 1e-12

int get_right_down_point(double ** arr, int n){
    int tmp_i = 0;
    double x_min = arr[0][0];
    double y_min = arr[0][1];
    for (int i = 0; i < n; i++){
        if (arr[i][0] > x_min + epsilon){
            tmp_i = i;
            x_min = arr[i][0];
            y_min = arr[i][1];
        }
        else{
            if (fabs(arr[i][0] - x_min) <=epsilon){
                if (arr[i][1] < y_min - epsilon){
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


double ** Jarvis_alg(double ** arr, int n, int * hull_size) {
    int *indx = (int *) malloc(n * sizeof(int));
    int count_points_convex_hull = 0;
    int start = get_right_down_point(arr, n);

    int curr = start;

    int flag = 0;
    while (flag == 0 && count_points_convex_hull < n){
        indx[count_points_convex_hull] = curr;
        count_points_convex_hull++;

        int next = -1;
        for (int i = 0; i < n; i++){
            if (i != curr){
                if (next == -1){
                    next = i;
                }
                else{
                    if (vec_mult_sign((arr[curr]), arr[next], arr[i]) < 0){
                        next = i;
                    }
                    else{
                        if (fabs(vec_mult_sign((arr[curr]), arr[next], arr[i])) == 0){
                            if (dist(arr[curr], arr[i]) > dist(arr[curr], arr[next]) + epsilon){
                                next = i;
                            }
                        }
                    }
                }
            }
        }
        if (next == -1){
            return NULL;
        }
        curr = next;
        if (curr == start){
            flag = 1;
        }
    }

    double **points = (double**)malloc(count_points_convex_hull * sizeof (double*));
    for (int i = 0; i < count_points_convex_hull; i++){
        points[i] = malloc(2 * sizeof(double));
        points[i][0] = arr[indx[i]][0];
        points[i][1] = arr[indx[i]][1];
    }
    free(indx);
    *hull_size = count_points_convex_hull;
    return points;
}


int main(void){
    int n = 0;
    scanf("%d", &n);
    double **arr = (double**)malloc(n * sizeof(double*));
    int i = 0;
    for (i = 0; i < n; i++){
        arr[i] = (double *)malloc(2 * sizeof(double));
        scanf("%lf %lf", &arr[i][0], &arr[i][1]);
    }

    double ** uniq = (double **) malloc(n * sizeof (double *));
    int size = 0;
    for (int i = 0; i< n; i++){
        int flag = 0;
        for (int j = 0; j < size && flag == 0; j++){
            if (fabs(arr[i][0] - uniq[j][0]) < epsilon && (fabs(arr[i][1] - uniq[j][1]) < epsilon)){
                flag = 1;
            }
        }
        if (!flag){
            uniq[size] = (double *)malloc(2 * sizeof(double ));
            uniq[size][0] = arr[i][0];
            uniq[size][1] = arr[i][1];
            size++;
        }
    }


    if (size == 1){
        printf("0.0\n");
        free(uniq[0]);
        free(uniq);
        for (int i = 0 ; i < n ; i++){
            free(arr[i]);
        }
        free(arr);
        return 0;
    }
    if (size == 2){
        int i = get_right_down_point(uniq, size);
        double a[2] = {uniq[i][0], uniq[i][1]};
        double b[2] = {uniq[(i+1)%2][0], uniq[(i+1)%2][1]};
        printf("%lf\n", dist(a, b));
        free(uniq[0]);
        free(uniq[1]);
        for (int i = 0 ; i < n ; i++){
            free(arr[i]);
        }
        free(arr);
        free(uniq);
        return 0;
    }

    int hull_size = 0;
    double ** convex_hull = Jarvis_alg(uniq, size, &hull_size);
    if (convex_hull == NULL){
        for (int i = 0; i < n; i++){
            free(arr[i]);
        }
        for (int i = 0; i < size; i++){
            free(uniq[i]);
        }
        free(uniq);
        return 0;
    }
    double per = 0.0;
    for (int i = 0; i < hull_size; i++){
        double a[2] = {convex_hull[i][0], convex_hull[i][1]};
        double b[2] = {convex_hull[(i + 1) % hull_size][0], convex_hull[(i + 1) % hull_size][1]};
        per+= dist(a, b);
    }
    for (int i = 0; i < hull_size; i++) {
        free(convex_hull[i]);
    }
    printf("%lf\n", per);
    free(convex_hull);
    for (int i = 0; i < n; i++){
        free(arr[i]);
    }
    for (int i = 0; i < size; i++){
        free(uniq[i]);
    }
    free(uniq);
    return 0;
}