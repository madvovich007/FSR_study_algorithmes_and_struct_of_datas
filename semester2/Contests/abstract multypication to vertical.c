#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define epsilon 1e-10

struct list{
    int value;
    struct list * left_child;
    struct list * right_child;
};


typedef struct event{
    double x;
    int type;
    int index1;
    int index2;
}event;


typedef struct all{
    double *x1, *y1, *x2, *y2;
    int *indx;
    double cur_x;
}all;


double get_y(int id, double x, all *c) {
    return c->x1[id];
}


void insert(struct list** head, int value, all *c) {
    if (!*head){
        struct list* new_list = (struct list*)malloc(sizeof(struct list));
        new_list->value = value;
        new_list->left_child = NULL;
        new_list->right_child = NULL;
        *head = new_list;
    }
    else{
        double y_new = get_y(value, c->cur_x, c);
        double y_tmp = get_y((*head)->value, c->cur_x, c);
        if (y_new < y_tmp - epsilon){
            insert(&((*head)->left_child), value, c);
        }
        else if (y_new > y_tmp + epsilon){
            insert(&((*head)->right_child), value, c);
        }
        else{
            if (value < (*head)->value){
                insert(&((*head)->left_child), value, c);
            }
            else{
                insert(&((*head)->right_child), value, c);
            }
        }
    }
}

void clear(struct list* head){
    if (head == NULL){
        return;
    }
    clear(head -> left_child);
    clear(head -> right_child);
    free(head);
}


struct list* delete(struct list * head, int value, all *c) {
    if (head == NULL){
        return head;
    }
    double y_val = get_y(value, c->cur_x, c);
    double y_head = get_y(head->value, c->cur_x, c);

    if (value != head->value){
        if (y_val < y_head - epsilon || (fabs(y_val - y_head) < epsilon && value < head->value)) {
            head->left_child = delete(head->left_child, value, c);
        }
        else{
            head->right_child = delete(head->right_child, value, c);
        }
    }
    else{
        if (head->left_child == NULL){
            struct list* tmp = head->right_child;
            free(head);
            return tmp;
        }
        else if (head->right_child == NULL){
            struct list* tmp = head->left_child;
            free(head);
            return tmp;
        }
        struct list* cur = head->right_child;
        while (cur && cur->left_child != NULL){
            cur = cur->left_child;
        }
        head->value = cur->value;
        head->right_child = delete(head->right_child, cur->value, c);
    }
    return head;
}

int comp_event(event * a, event * b){
    if (fabs(a->x - b->x) > epsilon){
        if (a -> x < b->x){
            return -1;
        }
        return 1;
    }
    if (a->type != b->type){
        return a->type - b->type;
    }
    if (a->index1 != b->index1){
        return a->index1 - b->index1;
    }
    return 0;
}


int get_prev(struct list* head, int value, all *c){
    int result = -1;
    double y_val = get_y(value, c->cur_x, c);
    while (head){
        double y_h = get_y(head->value, c->cur_x, c);
        if (y_h < y_val - epsilon || (fabs(y_h - y_val) < epsilon && head->value < value)){
            result = head->value; head = head->right_child;
        }
        else{
            head = head->left_child;
        }
    }
    return result;
}

int get_next(struct list* head, int value, all *c){
    int result = -1;
    double y_val = get_y(value, c->cur_x, c);
    while (head){
        double y_h = get_y(head->value, c->cur_x, c);
        if (y_h > y_val + epsilon || (fabs(y_h - y_val) < epsilon && head->value > value)){
            result = head->value; head = head->left_child;
        }
        else{
            head = head->right_child;
        }
    }
    return result;
}


void up_screening(event heap[], int tmp_index){
    if (tmp_index == 0){
        return;
    }
    int parent = (tmp_index - 1)/2;
    if (comp_event(&heap[tmp_index], &heap[parent]) < 0){
        event tmp = heap[tmp_index];
        heap[tmp_index] = heap[parent];
        heap[parent] = tmp;
        up_screening(heap, parent);
    }
    return;
}

void ins(event heap[], int * size, event value){
    heap[*size] = value;
    up_screening(heap, *size);
    (*size)++;
    return;
}

void down_screening(event heap[], int tmp_index, int size){
    int maxi = tmp_index;
    if (size != 0){
        if (2 * tmp_index + 1 < size && comp_event(&heap[2 * tmp_index + 1], &heap[maxi]) < 0){
            maxi = 2 * tmp_index + 1;
        }
        if (2 * tmp_index + 2 < size && comp_event(&heap[2 * tmp_index + 2], &heap[maxi]) < 0){
            maxi = 2 * tmp_index + 2;
        }
        if (maxi != tmp_index){
            event tmp = heap[tmp_index];
            heap[tmp_index] = heap[maxi];
            heap[maxi] = tmp;
            return down_screening(heap, maxi, size);
        }
    }
    return;
}

event pop(event heap[], int* size){
    event tmp = heap[0];
    heap[0] = heap[*size - 1];
    (*size)--;
    down_screening(heap, 0, *size);
    return tmp;
}



double max(double a, double b){
    if (a > b){
        return a;
    }
    return b;
}

double min(double a, double b){
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
    return sqrt((x1 - x2) * (x1- x2) + (y1 - y2) * (y1 - y2));
}


int on_seg (double x1, double y1, double x2, double y2, double x3, double y3){
    return (x3 >= min(x1, x2) &&
            x3 <= max(x1, x2) &&
            y3 >= min(y1, y2) &&
            y3 <= max(y1, y2));
}


int get_intersect_x(int i, int j, all *c, double *x) {
    double x1 = c->x1[i], y1 = c->y1[i];
    double x2 = c->x2[i], y2 = c->y2[i];
    double x3 = c->x1[j], y3 = c->y1[j];
    double x4 = c->x2[j], y4 = c->y2[j];
    double tmp = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    if (fabs(tmp) < epsilon){
        return 0;
    }
    *x = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4)) / tmp;
    return 1;
}

void add_area(double *area, int **cross, all *c, int i, int j){
    if (i == -1 || j == -1 || i == j){
        return;
    }
    if (cross[i][j]){
        return;
    }
    double y_1 = c->y1[i], y_top_1 = c->y2[i];
    double y_2 = c->y1[j], y_top_2 = c->y2[j];
    double width = min(y_top_1, y_top_2) - max(y_1, y_2);
    if (width <= epsilon){
        return;
    }
    double height = 0.0;
    if (c->x1[i] > c->x1[j]){
        height = c->x1[i] - c->x2[j];
    }
    else{
        height = c->x1[j] - c->x2[i];
    }
    if (height <= epsilon){
        return;
    }
    cross[i][j] = 1;
    cross[j][i] = 1;
    *area += width * height;
}


void Bentley_Ottman_alg(int n, all * c, double *area, int** cross){
    event * heap = malloc(10 * n * n * sizeof (event));
    int size = 0;
    for (int i = 0; i < n; i++){
        ins(heap, &size, (event){c->y1[i], 0, i, -1});;
        ins(heap, &size, (event){c->y2[i], 1, i, -1});
    }

    struct list * head = NULL;
    while (size > 0){
        event curr_ev = pop(heap, &size);
        c->cur_x = curr_ev.x;
        if (curr_ev.type == 0){
            insert(&head, curr_ev.index1, c);
        }
        if (curr_ev.type == 1){
            int next = get_next(head, curr_ev.index1, c);
            int prev = get_prev(head, curr_ev.index1, c);
            add_area(area, cross, c, curr_ev.index1, next);
            add_area(area, cross, c, curr_ev.index1, prev);
            head = delete(head, curr_ev.index1, c);
        }
    }
    clear(head);
    free(heap);
}


int main(void) {
    struct list* head = NULL;
    int n = 0;
    scanf("%d", &n);
    int * indx = (int*)malloc(n * sizeof(int));
    double * x1 = (double *)malloc(n * sizeof(double));
    double * y1 = (double *)malloc(n * sizeof(double));
    double * x2 = (double *)malloc(n * sizeof(double));
    double * y2 = (double *)malloc(n * sizeof(double));
    all c;
    c.indx = indx;
    c.x1 = x1;
    c.y1 = y1;
    c.x2 = x2;
    c.y2 = y2;
    for (int i = 0; i < n; i++){
        double a, b, c, d;
        scanf("%lf %lf %lf %lf", &a, &b, &c, &d);
        x1[i] = a;
        x2[i] = a + c;
        y1[i] = b;
        y2[i] = b + d;
        indx[i] = i;
    }
    double area = 0.0;
    for (int i = 0; i < n; i++) {
        area += (c.x2[i] - c.x1[i]) * (c.y2[i] - c.y1[i]);
    }

    int ** cross = malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        cross[i] = calloc(n, sizeof(int));
    }
    Bentley_Ottman_alg(n, &c, &area, cross);

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            double x1 = min(c.x2[i], c.x2[j]) - max(c.x1[i], c.x1[j]);
            double y1 = min(c.y2[i], c.y2[j]) - max(c.y1[i], c.y1[j]);
            if (x1 > epsilon && y1 > epsilon) {
                area -= x1 * y1;
            }
        }
    }

    printf("%lf", area *0.0001);
    for (int i = 0; i < n; i++){
        free(cross[i]);
    }
    free(cross);
    free(indx);
    free(x1);
    free(y1);
    free(x2);
    free(y2);
    clear(head);
    return 0;
}