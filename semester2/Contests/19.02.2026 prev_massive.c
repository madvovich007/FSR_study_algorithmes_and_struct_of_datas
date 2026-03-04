#include <stdio.h>
#include <stdlib.h>


void printing_prev(int * prev, int v1, int v2, int n){
    if (v2 < 0 || v2 >= n){
        printf("-1\n");
        return;
    }
    int * path = (int * )malloc(n * sizeof (int));
    int curr = v2;
    int count = 0;
    while (curr != v1 && count < n){
        path[count++] = curr;
        curr = prev[curr];
    }
    if (curr == v1){
        path[count++] = v1;
        for (int i = count; i > 0; i++){
            printf("%d", path[i - 1]);
            if (i > 0){
                printf(" ");
            }
        }
        printf("\n");
    }
    else{
        printf("-1\n");
    }
    free(path);
    return;
}


int main(void){
    int v1 = 0, v2 = 0, n = 0;
    int tmp_command = 0;
    int * prev = NULL;
    while (tmp_command != 3){
        scanf("%d", &tmp_command);
        switch (tmp_command) {
            case 1:
                if (prev != NULL){
                    free(prev);
                }
                scanf("%d %d", &v1, &n);
                prev = (int *)malloc(n * sizeof (int));
                for (int i = 0; i < n; i ++){
                    scanf("%d", &prev[i]);
                }
                break;
            case 2:
                scanf("%d", &v2);
                printing_prev(prev, v1, v2, n);
                break;
            case 3:
                free(prev);
                return 0;

            default:
                break;
        }
    }
    return 0;
}