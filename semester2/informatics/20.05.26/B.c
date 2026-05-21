#include <stdio.h>
#include <stdlib.h>
#define maxi_l 10000
int len(char* arr){
    int c = 0;
    while (arr[c] != '\0'){
        c++;
    }
    return c;
}

int comp(char * a, char * b){
    int lena = len(a);
    int lenb = len(b);
    for (int i = 0; i < lena + lenb; i++){
        char c1, c2;
        if (i < lena){
            c1 = a[i];
        }
        else{
            c1 = b[i - lena];
        }
        if (i < lenb){
            c2 = b[i];
        }
        else{
            c2 = a[i - lenb];
        }
        if (c1 != c2){
            return c1 - c2;
        }
    }
    return 0;
}

void swap(char *s1, char *s2){
    char tmp[105];
    int i = 0;
    while (s1[i] != '\0'){
        tmp[i] = s1[i]; ++i;
    }
    tmp[i] = '\0';
    i = 0;
    while (s2[i] != '\0'){
        s1[i] = s2[i]; ++i;
    }
    s1[i] = '\0';
    i = 0;
    while (tmp[i] != '\0'){
        s2[i] = tmp[i]; ++i;
    }
    s2[i] = '\0';
}


int main(void){
    char ** arr = (char**)malloc(100 * sizeof (char *));
    int n = 0;
    int flag = 1;
    for (int i = 0; i < 100 && flag; i++) {
        arr[i] = (char*)malloc(101);
        if (scanf("%s", arr[i]) == 1){
            n++;
        }
        else{
            free(arr[i]);
            flag = 0;
        }
    }
    for (int i = 0; i < n - 1; ++i){
        for (int j = 0; j < n - 1 - i; ++j){
            if (comp(arr[j], arr[j+1]) < 0){
                swap(arr[j], arr[j+1]);
            }
        }
    }
    for (int i = 0; i < n; i++){
        printf("%s", arr[i]);
    }
    for (int i = 0 ; i < n; i++){
        free(arr[i]);
    }
    free(arr);
    return 0;
}