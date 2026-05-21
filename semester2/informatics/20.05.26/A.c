#include <stdio.h>

#define maxi_l 10000

int equal_of_substr(char* str1, char* str2, int ind, int len){
    if (str1 == NULL || str2 == NULL) {
        return -1;
    }
    int i = 0;
    while (i < len && str1[i] == str2[(ind + i) % len]){
        i++;
    }
    if (i == len){
        return 1;
    }
    return 0;
}

int main(void){
    int len = 0;
    char str1[maxi_l];
    char str2[maxi_l];
    while (scanf("%c", str1 + len) != EOF && str1[len] != '\n'){
        len++;
    }
    for (int i = 0; i < len; i++){
        scanf("%c", str2 + i);
    }
    int i = 0;
    while (i < len && !equal_of_substr(str1, str2, i, len)){
        i++;
    }
    if (i < len){
        printf("%d", i);
    }
    else{
        printf("-1");
    }
    return 0;
}