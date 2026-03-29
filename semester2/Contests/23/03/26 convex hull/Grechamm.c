#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define infinity 1e7
#define epsilon 1e-12
#define pi 3.14159265358979323846


typedef struct point{
    int index;
    double angle;
    double dist;
} point;


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


int compare (point * a, point * b){
    if (fabs(a->angle - b->angle) > epsilon){
        if (a->angle > b->angle){
            return 1;
        }
        else{
            return -1;
        }
    }
    else{
        if (fabs(a->dist - b->dist) > epsilon){
            if (a->dist < b->dist){
                return -1;
            }
            else{
                return 1;
            }
        }
        else{
            return 0;
        }
    }
}


void quick_sort(point arr[], int n){
    if (n <= 1){
        return;
    }
    point sup = arr[n / 2];
    int i = 0;
    int j = n - 1;
    if (n > 1){
        while (i <= j){
            while (compare(&arr[i], &sup) < 0){
                i++;
            }
            while (compare(&arr[j], &sup) > 0){
                j--;
            }
            if (i <= j){
                point tmp = arr[i];
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


double ** Graham_alg(double ** arr, int n, int * hull_size) {

    int start = get_right_down_point(arr, n);
    point *points = (point *)malloc((n - 1) * sizeof (point));
    int counter = 0;
    for (int i = 0; i < n; i++){
        if (i != start){
            points[counter].index = i;
            points[counter].angle = atan2((arr[i][1] - arr[start][1]), (arr[i][0] - arr[start][0]));
            if (points[counter].angle < 0){
                points[counter].angle += 2 * pi;
            }
            points[counter].dist = dist(arr[i], arr[start]);
            counter++;
        }
    }
    quick_sort(points, n - 1);

    // удалем колинеарные, оставляя самую дальнюю

    int* new_index = (int *)malloc(n * sizeof (int));
    int new_counter = 0;
    new_index[new_counter++] = start;
    double maxid = points[0].dist;
    double prev_ang = points[0].angle;
    int tmp_ind = points[0].index;
    for (int i = 1; i < n - 1; i++){
        if (fabs(points[i].angle - prev_ang) < epsilon){
            if (points[i].dist > maxid + epsilon){
                maxid = points[i].dist;
                tmp_ind = points[i]. index;
            }
        }
        else{
            new_index [new_counter++] = tmp_ind;
            prev_ang = points[i].angle;
            maxid = points[i].dist;
            tmp_ind = points[i]. index;
        }
    }
    new_index[new_counter++] = tmp_ind;

    int *stack = (int*)malloc(new_counter * sizeof (int));
    int top = 0;
    stack[0] = new_index[0];
    if (new_counter >1){
        stack[++top] = new_index[1];
    }
    for (int i = 2; i< new_counter; i++){
        while(top >=1 && vec_mult_sign(arr[stack[top - 1]], arr[stack[top]], arr[new_index[i]]) <= 0){
            top --;
        }
        stack[++top] = new_index[i];
    }
    *hull_size = top + 1;



    double **points2 = (double**)malloc(*hull_size * sizeof (double*));
    for (int i = 0; i < *hull_size; i++){
        points2[i] = malloc(2 * sizeof(double));
        points2[i][0] = arr[stack[i]][0];
        points2[i][1] = arr[stack[i]][1];
    }
    free(new_index);
    free(stack);
    free(points);
    return points2;
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
        printf("%lf %lf", uniq[0][0], uniq[0][1]);
        free(uniq[0]);
        free(uniq);
        free(arr);
        return 0;
    }
    if (size == 2){
        int i = get_right_down_point(uniq, size);
        printf("%lf %lf\n", uniq[i][0], uniq[i][1]);
        printf("%lf %lf", uniq[(i+1)%2][0], uniq[(i+1)%2][1]);
        free(uniq[0]);
        free(uniq[1]);
        free(arr[0]);
        free(arr[1]);
        free(arr);
        free(uniq);
        return 0;
    }

    int hull_size = 0;
    double ** convex_hull = Graham_alg(uniq, size, &hull_size);
    if (convex_hull == NULL){
        return 0;
    }

    for (int i = 0; i < hull_size; i++){
        printf("%lf %lf\n", convex_hull[i][0], convex_hull[i][1]);
        free(convex_hull[i]);
    }
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