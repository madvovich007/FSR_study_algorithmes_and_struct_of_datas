#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define epsilon 1e-10


int max(int a, int b){
    if (a > b){
        return a;
    }
    return b;
}

int min(int a, int b){
    if (a < b){
        return a;
    }
    return b;
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

int vec_mult_sign(double x1, double y1, double x2, double y2, double x3, double y3){
    return signum((x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1));
}


double dist(int x1, int y1, int x2, int y2){
    return sqrt((x1 - x2) * (x1- x2) + (y1 - y2) * (y1 - y2));
}


int on_seg (int x1, int y1, int x2, int y2, int x3, int y3){
    return (x3 >= min(x1, x2) &&
            x3 <= max(x1, x2) &&
            y3 >= min(y1, y2) &&
            y3 <= max(y1, y2));
}

int main(void){
    int x1, y1, x2, y2, x3, y3, x4, y4;
    scanf("%d %d %d %d %d %d %d %d", &x1, &y1, &x2, &y2, &x3, &y3, &x4, &y4);
    int a = vec_mult_sign(x1, y1, x2, y2, x3, y3);
    int b = vec_mult_sign(x1, y1, x2, y2, x4, y4);
    int c = vec_mult_sign(x3, y3, x4, y4, x1, y1);
    int d = vec_mult_sign(x3, y3, x4, y4, x2, y2);
    if (a != b && c != d){
        printf("YES");
        return 0;
    }
    if (a == 0 && on_seg(x1, y1, x2, y2, x3, y3) ||
        b == 0 && on_seg(x1, y1, x2, y2, x4, y4) ||
        c == 0 && on_seg(x3, y3, x4, y4, x1, y1) ||
        a == 0 && on_seg(x3, y3, x4, y4, x2, y2)){
        printf("YES");
        return 0;
    }

    printf("NO");
    return 0;
}
