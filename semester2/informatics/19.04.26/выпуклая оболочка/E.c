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
    double maxi = 1e-11;
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

    if (k > 1){
        double wanted_sq = total / k;
        point start = convex_hull[0];
        for (int i = 1; i < k ; i++){
            double need_sq = i * wanted_sq;
            double collected = 0;
            point cut_points = start;
            int flag = 0;
            for (int j = 1; j < n && flag == 0; j++){
                point a = convex_hull[j];
                point b = convex_hull[(j + 1) % n];
                double triangle_sq = fabs( 0.5 * ((a.x - start.x) * (b.y - start.y) - (a.y - start.y) * (b.x - start.x)));
                if (collected + triangle_sq >= need_sq - epsilon){
                    cut_points.x = a.x + (need_sq - collected) / triangle_sq * (b.x - a.x);
                    cut_points.y = a.y + (need_sq - collected) / triangle_sq * (b.y - a.y);
                    flag = 1;
                }
            collected += triangle_sq;
            }
            printf("%.10lf %.10lf %.10lf %.10lf\n",start.x, start.y, cut_points.x, cut_points.y);
        }
    }
    free(convex_hull);
    return 0;
}