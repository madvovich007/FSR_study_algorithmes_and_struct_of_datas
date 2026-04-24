#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define infinity 1e7
#define epsilon 1e-12


double dist(double * a, double* b){
    return sqrt((a[0] - b[0])* (a[0] - b[0]) + (a[1] - b[1])* (a[1] - b[1]));
}

void intersect(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, double *res) {
    double a1 = y2 - y1;
    double b1 = x1 - x2;
    double c1 = a1 * x1 + b1 * y1;
    double a2 = y4 - y3;
    double b2 = x3 - x4;
    double c2 = a2 * x3 + b2 * y3;
    double d = a1 * b2 - a2 * b1;
    res[0] = (b2 * c1 - b1 * c2) / d;
    res[1] = (a1 * c2 - a2 * c1) / d;
}


int cut_pol(double** pol, int n, double a, double b, double c, double ** next_pol){
    int m = 0;
    for (int i = 0; i < n; i++){
        double tmp1 = a * pol[i][0] + b * pol[i][1] + c;
        double tmp2 = a * pol[(i + 1) % n][0] + b * pol[(i + 1) % n][1] + c;
        if (tmp1 > -epsilon){
            if (tmp2 > -epsilon){
                next_pol[m][0] = pol[(i + 1) % n][0];
                next_pol[m][1] = pol[(i + 1) % n][1];
                m++;
            }
            else{
                double res[2];
                double no = a * a + b * b;
                double x0 = - a * c / no;
                double y0 = - b * c / no;
                intersect(pol[i][0], pol[i][1], pol[(i + 1) % n][0], pol[(i + 1) % n][1], x0, y0, x0 - b, y0 + a, res);
                next_pol[m][0] = res[0];
                next_pol[m][1] = res[1];
                m++;
            }
        }
        else{
            if (tmp2 > -epsilon){
                double res[2];
                double no = a * a + b * b;
                double x0 = - a * c / no;
                double y0 = - b * c / no;
                intersect(pol[i][0], pol[i][1], pol[(i + 1) % n][0], pol[(i + 1) % n][1], x0, y0, x0 - b, y0 + a, res);
                next_pol[m][0] = res[0];
                next_pol[m][1] = res[1];
                m++;
                next_pol[m][0] = pol[(i + 1) % n][0];
                next_pol[m][1] = pol[(i + 1) % n][1];
                m++;
            }
        }
    }
    return m;
}


int main(void){
    int n = 0;
    double r = 0;
    scanf("%d %lf", &n, &r);
    double ** pol = (double ** )malloc(2 *n * sizeof(double *));
    double ** temp = (double ** )malloc(2 * n * sizeof(double *));

    for (int i = 0; i < n; i++){
        pol[i] = (double *)malloc(2 * sizeof( double));
        temp[i] = (double *)malloc(2 * sizeof( double));
    }
    for (int i = 0; i < n; i++) {
        scanf("%lf %lf", &pol[i][0], &pol[i][1]);
    }

    double ** orig = (double **)malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++){
        orig[i] = (double *)malloc(2 * sizeof(double));
        orig[i][0] = pol[i][0];
        orig[i][1] = pol[i][1];
    }

    int curr_n = n;
    for (int i = 0; i < n; i++){
        double x1 = orig[i][0];
        double y1 = orig[i][1];
        double x2 = orig[(i + 1) % n][0];
        double y2 = orig[(i + 1) % n][1];
        double a[2]= {x1, y1};
        double b[2] = {x2, y2};
        double d = dist(a, b);
        curr_n = cut_pol(pol, curr_n, (y2 - y1) / d, -(x2 - x1) / d, -((y2 - y1) / d * x1 - (x2 - x1) / d * y1 + r), temp);
        for (int j = 0; j < curr_n; j++){
            pol[j][0] = temp[j][0];
            pol[j][1] = temp[j][1];
        }
    }







    double maxi = -epsilon;
    double b_v1[2];
    double b_v2[2];
    if (curr_n > 0){
        b_v1[0] = pol[0][0];
        b_v1[1] = pol[0][1];
        b_v2[0] = pol[0][0];
        b_v2[1] = pol[0][1];
        for (int i = 0; i < curr_n; i++){
            for (int j = i; j < curr_n; j++){
                if (dist(pol[i], pol[j]) > maxi){
                    maxi = dist(pol[i], pol[j]);
                    b_v1[0] = pol[i][0]; b_v1[1] = pol[i][1];
                    b_v2[0] = pol[j][0]; b_v2[1] = pol[j][1];
                }
            }
        }
    }
    else{
        b_v1[0] = b_v2[0] = pol[0][0];
        b_v1[1] = b_v2[1] = pol[0][1];
    }

    printf("%lf %lf %lf %lf", b_v1[0], b_v1[1], b_v2[0], b_v2[1]);
    for (int i = 0; i < 2 * n; i++){
        free(pol[i]);
        free(temp[i]);
    }
    for (int i = 0 ; i < n; i++){
        free(orig[i]);
    }
    free(pol);
    free(temp);
    free(orig);
    return 0;
}