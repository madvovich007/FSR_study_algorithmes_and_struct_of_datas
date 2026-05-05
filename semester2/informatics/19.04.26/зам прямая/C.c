#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define epsilon 1e-9

typedef struct {
    int l, r, index;
}segment;


int compare_seg(const segment *a, const segment *b) {
    if (a -> l != b -> l)
        return (a -> l > b -> l) ? 1 : -1;
    if (a -> r != b -> r) {
        return (a -> r < b -> r) ? 1 : -1;
    }
    return 0;
}


void swap(segment *a, segment *b){
    segment tmp = *a;
    *a = *b;
    *b = tmp;
}


void quick_sort(segment arr[], int n){
    if (n <= 1){
        return;
    }
    segment sup = arr[n / 2];
    int i = 0;
    int j = n - 1;
    if (n > 1){
        while (i <= j){
            while (compare_seg(&arr[i], &sup) < 0){
                i++;
            }
            while (compare_seg(&arr[j], &sup) > 0){
                j--;
            }
            if (i <= j){
                swap(&arr[i], &arr[j]);
                i++;
                j--;
            }
        }
        quick_sort(arr, j + 1);
        quick_sort(arr + i, n - i);
    }
}


int main(void) {
    int n;
    scanf("%d", &n);
    segment *segments = (segment*)malloc(n * sizeof(segment));
    for (int i = 0; i < n; ++i) {
        scanf("%d %d", &segments[i].l, &segments[i].r);
        segments[i].index = i;
    }
    quick_sort(segments, n);
    int *answer = (int*)calloc(n, sizeof(int));
    int *stack = (int*)malloc(n * sizeof(int));
    int top = -1;
    for (int i = 0; i < n; ++i) {
        while (top >= 0 && segments[stack[top]].r < segments[i].r)
            top--;
        if (top >= 0){
            answer[segments[i].index] = segments[stack[top]].index + 1;
        }
        else{
            answer[segments[i].index] = 0;
        }
        stack[++top] = i;
    }
    for (int i = 0; i < n; ++i)
        printf("%d ", answer[i]);
    free(segments);
    free(answer);
    free(stack);
    return 0;
}