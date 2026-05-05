#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


#define epsilon 1e-10


int main(void){
    int x, y;
    scanf("%d", &x);
    scanf("%d", &y);
    char *word = (char *)malloc(x * sizeof(char));
    char *text = (char *)malloc(y * sizeof(char));
    char c;
    scanf("%c", &c);
    for (int i = 0; i < x; i++){
        scanf("%c", &word[i]);
    }
    scanf("%c", &c);
    for (int i = 0; i < y; i++){
        scanf("%c", &text[i]);
    }
    int counter1 = x;
    int counter2 = y;
    char * arr = (char *)malloc((counter1 + counter2 + 1) * sizeof(char));
    for (int i = 0; i < counter1; i++){
        arr[i] = word[i];
    }
    arr[counter1] = '.';
    for (int i = 0; i < counter2; i++){
        arr[counter1 + i + 1] = text[i];
    }
    int * ans = (int *) malloc((counter1 + 1 + counter2) * sizeof(int));
    ans[0] = 0;
    for(int i = 1; i < (counter1 + 1 + counter2); i++){
        int maxi = 0;
        int flag = 1;
        for (int k = i; k > 0 && flag; k--){
            int flag2 = 1;
            for (int j = 0; j < k && flag2; j++){
                flag2 = (arr[j] == arr[i - k + 1 + j]);
            }
            if (flag2){
                maxi = k;
                flag = 0;
            }
        }
        ans[i] = maxi;
    }
    int flag = 1;
    for (int i = counter1 + 1; i < counter1 + 1 + counter2 && flag; i++){
        if (ans[i] == counter1){
            printf("%d", i - 2 * counter1);
            flag = 0;
        }
    }
    if (flag){
        printf("-1");
    }
    free(ans);
    free(arr);
    free(text);
    free(word);
    return 0;
}