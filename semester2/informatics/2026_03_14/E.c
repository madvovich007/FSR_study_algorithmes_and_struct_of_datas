#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define infinity 100000000
#define maxi_towns 2000
#define epsilon 1e-12


typedef struct new_heap{
    int * heap;
    int * position;
    int size;
    int capacity;
}new_heap;

void up_screening(new_heap * heap, double *dist, int tmp_index){
    if (tmp_index == 0){
        return;
    }
    int parent = (tmp_index - 1)/2;
    if (dist[heap ->heap[parent]]> dist[heap ->heap[tmp_index]]){
        int tmp = heap ->heap[tmp_index];
        heap->heap[tmp_index] = heap ->heap[parent];
        heap ->heap[parent] = tmp;
        heap->position[heap->heap[tmp_index]] = tmp_index;
        heap->position[heap->heap[parent]] = parent;

        up_screening(heap, dist, parent);
    }
    return;
}

void ins(new_heap * heap, double * dist, int state){
    if (heap->position[state] != -1){
        int tmp_index = heap->position[state];
        up_screening(heap, dist, tmp_index);
    }
    else {
        heap->heap[heap->size] = state;
        heap->position[state] = heap->size;
        heap->size++;
        up_screening(heap, dist,heap->size - 1);
    }
    return;
}


int down_screening(new_heap * heap, int tmp_index,double * dist){
    int maxi = tmp_index;

    if (2 * tmp_index + 1 < heap->size && dist[heap->heap[2 * tmp_index + 1]] < dist[heap->heap[maxi]]){
        maxi = 2 * tmp_index + 1;
    }
    if (2 * tmp_index + 2 < heap->size && dist[heap->heap[2 * tmp_index + 2]] < dist[heap->heap[maxi]]){
        maxi = 2 * tmp_index + 2;
    }
    if (maxi != tmp_index){
        int tmp_val = heap->heap[tmp_index];
        heap->heap[tmp_index] = heap->heap[maxi];
        heap->heap[maxi] = tmp_val;
        heap->position[heap->heap[tmp_index]] = tmp_index;
        heap->position[heap->heap[maxi]] = maxi;
        down_screening(heap, maxi, dist);
    }
    return tmp_index;
}

int drawing_min(new_heap * heap, double * dist){
    int state;
    if (heap->size != 0){
        state = heap->heap[0];
        heap ->position[state] = -1;
        heap->size--;
        if (heap->size > 0) {
            heap->heap[0] = heap->heap[heap->size];
            heap->position[heap->heap[0]] = 0;
            down_screening(heap, 0, dist);
        }
        return state;
    }
    return -1;
}

void initialization_of_heap(new_heap * heap, int capacity){
    heap->heap = (int * )malloc (capacity * sizeof(int));
    heap->position = (int * )malloc (capacity * sizeof(int));
    for (int i = 0; i < capacity; i++){
        heap->position[i] = -1;
    }
    heap->size = 0;
    heap->capacity = capacity;
}

void freeing_of_heap(new_heap * heap){
    free(heap->heap);
    free(heap->position);
}

typedef struct edge{
    int to;
    double dist;
    struct edge * next;
} edge;

void dijkstra_alg(int n, edge ** edges, double * time_preparing, double * velocities, double * d, int * prev){
    char * visited = (char *)calloc(n * n,  sizeof (char));
    new_heap heap;
    initialization_of_heap(&heap, n * n);
    int size_of_heap = 0;
    for (int i = 0; i < n; i++){
        ins(&heap, d, i);
    }
    while (heap.size > 0){
        int curr = drawing_min(&heap, d);
        double curr_d = d[curr];
        if (!visited[curr]){
         visited[curr] = 1;
         int city = curr / n;
         int driver = curr %n;
         for (edge * i = edges[city]; i != NULL; i = i->next){
            int v = i->to;
            double weight = i->dist / velocities[driver];
            int new_state = v * n + driver;
            if (!visited[new_state] && d[new_state] > curr_d + weight){
                d[new_state] = curr_d + weight;
                prev[new_state] = curr;
                ins(&heap, d, new_state);
            }
        }

            if (city == driver){
                for (int j = 0; j < n; j++){
                    if (j != driver){
                        double weight = time_preparing[driver];
                        int new_state = city * n + j;
                        if (!visited[new_state] && d[new_state] > curr_d + weight){
                            d[new_state] = curr_d + weight;
                            prev[new_state] = curr;
                            ins(&heap, d, new_state);
                        }
                    }
                }
            }
        }
    }
    free(visited);
    freeing_of_heap(&heap);
}


int main(void){
    int  n = 0;
    scanf("%d", &n);
    double * time_of_preparing = (double *)malloc(n * sizeof (double ));
    double * velocities = (double *)malloc(n * sizeof (double ));
    for (int i = 0; i < n; i++){
        scanf("%lf %lf", &time_of_preparing[i], &velocities[i]);
    }
    edge ** mat = (edge**) calloc(n , sizeof(edge *));
    for (int i = 0; i < n - 1; i++){
        int x, y ,z;
        scanf("%d %d %d", &x, &y, &z);
        x--;
        y--;
        edge * e1 = (edge *)malloc(sizeof (edge));
        e1->to = y;
        e1->dist = z;
        e1->next = mat[x];
        mat[x] = e1;

        edge * e2 = (edge *)malloc(sizeof (edge));
        e2->to = x;
        e2->dist = z;
        e2->next = mat[y];
        mat[y] = e2;
    }

    double * d = (double *)malloc(n * n * sizeof (double));
    int * prev = (int *) malloc(n * n * sizeof(int));
    for (int i = 0; i < n * n; i++){
        d[i] = infinity;
        prev[i] = -1;
    }
    for (int i = 0; i < n; i++){
        d[i] = 0.0;
    }
    dijkstra_alg(n, mat, time_of_preparing, velocities, d, prev);


    double maxi_time = -1.0;
    int city = -1;
    for (int i = 1;  i < n; i++){
        int tmp_state = i * n + i;
        double total = time_of_preparing[i] + d[tmp_state];
        if (total > maxi_time + epsilon){
            maxi_time = total;
            city = i;
        }
    }

    if (n == 1){
        city = 0;
        maxi_time = 0.0;
    }

    int * path = (int *)malloc(maxi_towns * sizeof (int));
    int top = 0;
    int tmp_state = city * n + city;
    while (tmp_state != -1){
        int x = tmp_state / n;
        int y = tmp_state % n;
        if (x == y){
            path[top++] = x;
        }
        tmp_state = prev[tmp_state];
    }
    if (path[top - 1] != 0){
        path[top ++] = 0;
    }
    printf("%lf\n", maxi_time);
    for (int i = 0; i < top; i++){
        printf("%d", path[i] + 1);
        if (i < top - 1){
            printf(" ");
        }
    }


    for (int i = 0; i < n; i++){
        edge * tmp_e = mat[i];
        while (tmp_e != NULL){
            edge * new_tmp_e = tmp_e;
            tmp_e = new_tmp_e->next;
            free(new_tmp_e);
        }
    }
    free(time_of_preparing);
    free(velocities);
    free(mat);
    free(prev);
    free(d);
    free(path);

    return 0;
}