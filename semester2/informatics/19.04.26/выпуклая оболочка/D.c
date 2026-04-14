#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define infinity 1e7
#define epsilon 1e-12


typedef struct polygon{
    int k;
    double ** verts;
    double area;
} polygon;


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

int vec_mult_sign(double* a, double* b, double *c){
    return signum((b[0] - a[0]) * (c[1] - a[1]) - (b[1] - a[1]) * (c[0] - a[0]));
}


double dist(double * a, double* b){
    return sqrt((a[0] - b[0])* (a[0] - b[0]) + (a[1] - b[1])* (a[1] - b[1]));
}


int inside(double** vert, int k, double x_p, double y_p){
    double p[2] = {x_p, y_p};
    if (vec_mult_sign(vert[0], vert[1], p) < 0 || vec_mult_sign(vert[0], vert[k - 1], p) > 0){
        return 0;
    }
    int down_lim = 1;
    int high_lim = k - 2;
    int best = 1;
    while (down_lim <= high_lim){
        int cent_lim = (high_lim + down_lim) / 2;
        if (vec_mult_sign(vert[0], vert[cent_lim], p) >= 0){
            best = cent_lim;
            down_lim = cent_lim + 1;
        }
        else{
            high_lim = cent_lim - 1;
        }
    }
    return vec_mult_sign(vert[best], vert[(best + 1) % k], p) >= 0;
}
int comp_conv(polygon *a, polygon *b){
    double d = (a->area - b->area);
    return signum(d);
}

void quick_sort( polygon polygons[], int n){
    if (n <= 1){
        return;
    }
    double sup = polygons[n / 2].area;
    int i = 0;
    int j = n - 1;
    if (n > 1){
        while (i <= j){
            while (polygons[i].area < sup){
                i++;
            }
            while (polygons[j].area > sup){
                j--;
            }
            if (i <= j){
                polygon tmp = polygons[i];
                polygons[i] = polygons[j];
                polygons[j] = tmp;
                i++;
                j--;
            }
        }
        quick_sort(polygons, j + 1);
        quick_sort(polygons + i, n - i);
    }
}


int main(void){
    int n = 0;
    scanf("%d", &n);
    polygon * polygons = (polygon *)malloc(n * sizeof(polygon));
    for (int i = 0; i < n; i++){
        int k = 0;
        scanf("%d", &k);
        polygons[i].k = k;
        polygons[i].verts = (double **) malloc(k * sizeof (double *));
        double areas = 0;
        for (int j = 0; j < k; j++){
            polygons[i].verts[j] = (double *) malloc(2 * sizeof (double ));
            scanf("%lf %lf", &polygons[i].verts[j][0], &polygons[i].verts[j][1]);
        }
        for (int j = 0; j < k; j++){
            areas += (double)polygons[i].verts[j][0] * (double)polygons[i].verts[(j + 1) % k][1] -
                     (double)polygons[i].verts[(j + 1) % k][0] * (double)polygons[i].verts[j][1];
        }
        polygons[i].area = areas / 2.0;
    }
    quick_sort(polygons, n);

    int m = 0;
    scanf("%d", &m);
    char * indicators_beeing_layers = (char *) calloc(n, sizeof (char));
    for (int i = 0; i < m; i++){
        double x_p, y_p;
        scanf("%lf %lf", &x_p, &y_p);

        int down_lim = 0;
        int high_lim = n - 1;
        int indx = -1;
        while (down_lim <= high_lim){
            int cent_lim = (high_lim + down_lim) / 2;
            if (inside(polygons[cent_lim].verts, polygons[cent_lim].k, x_p, y_p)){
                indx = cent_lim;
                high_lim = cent_lim - 1;
            }
            else{
                down_lim = cent_lim + 1;
            }
        }
        if (indx != -1){
            indicators_beeing_layers[indx] = 1;
        }
    }
    double total = 0.0;
    for (int i = 0; i < n; i++){
        if (indicators_beeing_layers[i] == 1){
            if (i != 0) {
                total += (polygons[i].area - polygons[i - 1].area);
            }
            else{
                total += (polygons[i].area);
            }
        }
    }
    printf("%lf", total);
    for (int i = 0; i < n; i++){
        for (int j = 0; j < polygons[i].k; j++){
            free(polygons[i].verts[j]);
        }
        free(polygons[i].verts);
    }
    free(indicators_beeing_layers);
    free(polygons);
    return 0;
}