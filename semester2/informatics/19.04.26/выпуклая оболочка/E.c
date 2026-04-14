#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define infinity 1e7
#define epsilon 1e-12


typedef struct point{
    double x;
    double y;
} point;


double area_getting(point * points, int n){
    double area = 0.0;
    for (int i = 0; i < n; i++){
        area += (points[i].x * points[(i + 1) %n].y - points[(i + 1) % n].x * points[i].y);
    }
    return area / 2;
}


double left_sq(point * points, int n, double x){
    point * tmp = (point *) malloc(2 * n * sizeof(point));
    int counter = 0;
    for (int i = 0; i < n; i++){
        if (points[i].x <= x){
            if (points[(i + 1) % n].x <= x){
                tmp[counter++] = points[(i + 1) % n];
            }
            else{
                tmp[counter++] = (point){x, points[i].y + (points[(i + 1) % n].y - points[i].y) * (x - points[i].x) / (points[(i + 1) % n].x - points[i].x)};

            }
        }
        else{
            if (points[(i + 1) % n].x <= x){
                tmp[counter++] = (point){x, points[i].y + (points[(i + 1) % n].y - points[i].y) * (x - points[i].x) / (points[(i + 1) % n].x - points[i].x)};
                tmp[counter++] = points[(i + 1) % n];
            }
        }
    }
    double c = area_getting(tmp, counter);
    free(tmp);
    return c;
}


int main(void){
    int n = 0, k = 0;
    scanf("%d %d", &k, &n);
    point * convex_hull = (point *)malloc(n * sizeof(point));
    double mini = 1e10;
    double maxi = - 1e-11;
    for (int i = 0; i < n; i++){
        scanf("%lf %lf", &convex_hull[i].x, &convex_hull[i].y);
        if (convex_hull[i].x < mini){
            mini = convex_hull[i].x;
        }
        if (convex_hull[i].x > maxi){
            maxi = convex_hull[i].x;
        }
    }
    double total = area_getting(convex_hull, n);


    if (total < 0){
        for (int i = 0; i < n/2; i++){
            point temp = convex_hull[i];
            convex_hull[i] = convex_hull[n-1-i];
            convex_hull[n-1-i] = temp;
        }
        total *= (-1);
    }

    if (k > 1){
        double wanted_sq = total / k;
        for (int i = 1; i < k ; i++){
            double need_sq = i * wanted_sq;
            double left = mini, right = maxi;
            double collected = 0.0;
            while (right - left > epsilon){
                double mid = (left + right) / 2.0;
                collected = left_sq(convex_hull, n, mid);
                if (collected < need_sq){
                    left = mid;
                }
                else{
                    right = mid;
                }
            }
            double cut_x = (left + right) / 2.0;
            point tmp[2];
            int counter = 0;
            for (int j = 0; j < n && counter < 2; j++){
                point a = convex_hull[j];
                point b = convex_hull[(j + 1) % n];
                if ((a.x <= cut_x && b.x >= cut_x) || (a.x >= cut_x && b.x <= cut_x)){
                    if (fabs(a.x - b.x) < epsilon){
                        tmp[counter].x = cut_x;
                        tmp[counter].y = a.y;
                        counter++;
                        if (counter < 2){
                            tmp[counter].x = cut_x;
                            tmp[counter].y = b.y;
                            counter++;
                        }
                    }
                    else{
                        double t = (cut_x - a.x) / (b.x - a.x);
                        tmp[counter].x = cut_x;
                        tmp[counter].y = a.y + t * (b.y - a.y);
                        counter++;
                    }
                }
            }
            if (tmp[0].y > tmp[1].y){
                point tmp_2 = tmp[0];
                tmp[0] = tmp[1];
                tmp[1] = tmp_2;
            }
            printf("%.10lf %.10lf %.10lf %.10lf\n", tmp[0].x, tmp[0].y, tmp[1].x, tmp[1].y);
        }
    }
    free(convex_hull);
    return 0;
}