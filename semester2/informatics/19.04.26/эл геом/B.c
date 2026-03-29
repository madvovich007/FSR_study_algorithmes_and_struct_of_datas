#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define epsilon 1e-10

double dist(int x1, int y1, int x2, int y2){
    return sqrt((x1 - x2) * (x1- x2) + (y1 - y2) * (y1 - y2));
}

int main(void){
    int x_p, y_p, x1, y1, x2, y2;
    scanf("%d %d %d %d %d %d", &x_p, &y_p, &x1, &y1, &x2, &y2);
    int a = 0;
    int b = 0;
    int c = 0;
    if (fabs((dist(x_p, y_p, x1, y1) + dist(x_p, y_p, x2, y2)) - dist(x1, y1, x2, y2)) < epsilon){
        printf("YES");
    }
    else{
        printf("NO");
    }
    return 0;
}
