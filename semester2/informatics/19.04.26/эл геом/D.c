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


double dist(double x1, double y1, double x2, double y2){
    return(x1 - x2) * (x1- x2) + (y1 - y2) * (y1 - y2);
}


int on_seg (int x1, int y1, int x2, int y2, int x3, int y3){
    return (x3 >= min(x1, x2) &&
            x3 <= max(x1, x2) &&
            y3 >= min(y1, y2) &&
            y3 <= max(y1, y2));
}

int main(void){
    double x1, y1, r, x2, y2;
    scanf("%lf %lf %lf %lf %lf", &x1, &y1, &r, &x2, &y2);
    double distantion = dist(x1, y1, x2, y2);
    if (distantion < epsilon){
        printf("%d", 0);
        return 0;
    }
    if (distantion < r * r - epsilon){
        printf("%d", 0);
        return 0;
    }

    if (fabs(distantion - r * r) <= epsilon){
        printf("%d\n", 1);
        printf("%lf %lf", x2, y2);
        return 0;
    }

    printf("%d\n", 2);
    double d = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
    double tmp = r * r / d;
    double h = sqrt(r * r - tmp * tmp);
    double xm = x1 + (x2 - x1) * r * r / d / d;
    double ym = y1 + (y2 - y1) * r * r / d / d;
    double r1 = - (y2 - y1) * (h / d);
    double r2 = (x2 - x1) * (h / d);
    printf("%lf %lf\n", xm + r1, ym + r2);
    printf("%lf %lf\n", xm - r1, ym - r2);
    return 0;
}
