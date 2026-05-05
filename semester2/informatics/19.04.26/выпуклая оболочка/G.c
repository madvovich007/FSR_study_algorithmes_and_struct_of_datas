#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define infinity 1e7
#define epsilon 1e-12
typedef struct point{
    double x;
    double y;
} point;

int than(point a, point b) {
    if (!(fabs(a.x- b.x) < epsilon)) {
        return a.x < b.x;
    }
    if (!(fabs(a.y- b.y) < epsilon)) {
        return a.y < b.y;
    }
    return 0;
}

int swap(point *a, point *b) {
    point tmp = *a;
    *a = *b;
    *b = tmp;
    return 1;
}


void quick_sort(point *array, int size, int down, int up) {
    if (down >= up) {
        return;
    }
    point pivot = array[(up + down) / 2];
    int left = down;
    int right = up;
    while (left <= right) {
        while (than(array[left], pivot)) {
            left++;
        }
        while (than(pivot, array[right])) {
            right--;
        }
        if (left <= right) {
            swap(array + left, array + right);
            left++;
            right--;
        }
    }
    quick_sort(array, size, down, right);
    quick_sort(array, size, left, up);
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

int vec_mult_sign(point a, point b, point c) {
    return signum((b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x));
}

point*Graham_alg(point *arr, int n, int *hull_size){
    if (n <= 0) {
        *hull_size = 0;
        return NULL;
    }
    point * stack = malloc(n * sizeof(point));
    int top = 0;
    for (int i = 0; i < n; i++) {
        while (top > 1 && vec_mult_sign(stack[top-2], stack[top-1], arr[i]) <= 0)
            top--;
        stack[top++] = arr[i];
    }
    *hull_size = top;
    point* points2 = malloc(top * sizeof(point));
    for (int i = 0; i < top; i++) {
        points2[i].x = stack[i].x;
        points2[i].y = stack[i].y;
    }
    free(stack);
    return points2;
}


double max(double a, double b){
    if (a > b){
        return a;
    }
    return b;
}


int main(void){
    int n;
    scanf("%d", &n);

    point *all = NULL;
    int count = 0;

    for (int i = 0; i < n; i++) {
        int k;
        scanf("%d", &k);
        for (int j = 0; j < k + 1; j++) {
            double x, y;
            scanf("%lf %lf", &x, &y);
            all = (point*) realloc(all, (count + 1) * sizeof(point));
            all[count].x = fabs(x);
            all[count].y = y;
            count++;
        }
    }
    quick_sort(all, count, 0, count - 1);
    point* uniq = (point*) malloc(count * sizeof(point));
    int size = 0;
    if (count > 0){
        uniq[0].x = all[0].x;
        uniq[0].y = all[0].y;
        size = 1;
        for (int i = 1; i < count; i++){
            if (fabs(all[i].x - uniq[size - 1].x) > epsilon || fabs(all[i].y - uniq[size - 1].y) > epsilon) {
                uniq[size].x = all[i].x;
                uniq[size].y = all[i].y;
                size++;
            }
        }
    }

    int size_of_convex_hull = 0;
    point* convex_hull = Graham_alg(uniq, size, &size_of_convex_hull);

    double maxi_sq = 0.0;
    for (int i = 0; i < size_of_convex_hull - 1; i++){
        double x1 = convex_hull[i].x, y1 = convex_hull[i].y;
        double x2 = convex_hull[i + 1].x, y2 = convex_hull[i + 1].y;
        double A = y2 - y1;
        double B = x1 - x2;
        if (fabs(A) > epsilon && fabs(B) > epsilon){
            double C = -A * x1 - B * y1;
            double x0 = -C / A;
            double y0 = -C / B;
            maxi_sq = max(maxi_sq, (x0 * y0));
        }
    }
    printf("%lf", maxi_sq);
    free(uniq);
    free(all);
    free(convex_hull);
    return 0;
}