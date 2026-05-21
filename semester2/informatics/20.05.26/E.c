#include <stdio.h>
#include <stdlib.h>
#define maxi_l 1000000

int len(char* arr){
    int c = 0;
    while (arr[c] != '\0'){
        c++;
    }
    return c;
}

int main(void){
    char * a = (char*)malloc(maxi_l * sizeof(char));
    char * b = (char*)malloc(maxi_l * sizeof(char));
    scanf("%s %s", a, b);
    int n = len(a);
    char *new = (char*)malloc((2 * n + 1) * sizeof(char));

    int index = 0;
    for (int i = 0; i < n; ++i){
        new[index++] = a[i];
    }
    new[index++] = '#';
    for (int i = 0; i < n; ++i){
        new[index++] = b[i];
    }
    new[index] = '\0';
    int * ans = (int *) malloc((2 * n + 1) * sizeof(int));
    ans[0] = 0;
    for (int i = 1; i < 2 * n + 1; i++){
        int j = ans[i - 1];
        while (j > 0 && new[i] != new[j]){
            j = ans[j - 1];
        }
        if (new[i] == new[j]) {
            j++;
        }
        ans[i] = j;
    }
    printf("%d", n - ans[2 * n]);
    free(ans);
    free(new);
    free(a);
    free(b);
    return 0;
}