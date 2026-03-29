#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(void){
    int a, b, c, r;
    scanf("%d %d %d %d", &a, &b, &c, &r);
    printf("%d %d %lf", a, b, c + r * sqrt(a * a + b * b));
}