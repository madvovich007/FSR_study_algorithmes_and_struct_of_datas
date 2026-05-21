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
    char * arr = (char*)malloc(maxi_l * sizeof (char));
    scanf("%s", arr);
    int n = len(arr);

    int * ans = (int *) malloc(n * sizeof(int));
    ans[0] = 0;
    for (int i = 1; i < n; i++){
        int j = ans[i - 1];
        while (j > 0 && arr[i] != arr[j]){
            j = ans[j - 1];
        }
        if (arr[i] == arr[j]) {
            j++;
        }
        ans[i] = j;
    }
    int t = n - ans[n - 1];
    if (n % t == 0){
        printf("%d", n / t);
    }
    else{
        printf("1");
    }
    free(ans);
    free(arr);
    return 0;
}