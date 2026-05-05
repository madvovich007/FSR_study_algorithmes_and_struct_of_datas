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


typedef struct seg{
    int u, v;
}seg;

double get_y(int id, double x, all *c) {
    double res = c->y1[id];
    if (fabs(c->x2[id] - c->x1[id]) > epsilon) {
        double t = (x - c->x1[id]) / (c->x2[id] - c->x1[id]);
        res = c->y1[id] + (c->y2[id] - c->y1[id]) * t;
    }
    return res;
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
    int p_a;
    if (a->type == 1){
        p_a = 2;
    }
    else if (a->type == 2){
        p_a = 1;
    }
    else{
        p_a = 0;
    }

    int p_b;
    if (b->type == 1){
        p_b = 2;
    }
    else if (b->type == 2){
        p_b = 1;
    }
    else{
        p_b = 0;
    }
    return p_a - p_b;
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

void check_cross(double x1,double y1,double x2,double y2,double x3,double y3,double x4,double y4, int i, int j, int *counter, seg *ans){
    int a = vec_mult_sign(x1, y1, x2, y2, x3, y3);
    int b = vec_mult_sign(x1, y1, x2, y2, x4, y4);
    int c = vec_mult_sign(x3, y3, x4, y4, x1, y1);
    int d = vec_mult_sign(x3, y3, x4, y4, x2, y2);
    if (a != b && c != d){
        if (i < j){
            ans[*counter].u = i;
            ans[*counter].v = j;
        }
        else{
            ans[*counter].u = j;
            ans[*counter].v = i;
        }
        (*counter)++;
        return;
    }
    if ((a == 0 && on_seg(x1, y1, x2, y2, x3, y3)) ||
        (b == 0 && on_seg(x1, y1, x2, y2, x4, y4)) ||
        (c == 0 && on_seg(x3, y3, x4, y4, x1, y1)) ||
        (d == 0 && on_seg(x3, y3, x4, y4, x2, y2))){
        if (i < j){
            ans[*counter].u = i;
            ans[*counter].v = j;
        }
        else{
            ans[*counter].u = j;
            ans[*counter].v = i;
        }
        (*counter)++;
        return;
    }
    return;
}

int cmp_seg(seg* a, seg* b) {
    if (a->u != b->u){
        return a->u - b->u;
    }
    return a->v - b->v;
}

void quick_sort(seg arr[], int n){
    if (n <= 1){
        return;
    }
    seg sup = arr[n / 2];
    int i = 0;
    int j = n - 1;
    if (n > 1){
        while (i <= j){
            while (cmp_seg(&arr[i], &sup) < 0){
                i++;
            }
            while (cmp_seg(&arr[j], &sup) > 0){
                j--;
            }
            if (i <= j){
                seg tmp = arr[i];
                arr[i] = arr[j];
                arr[j] = tmp;
                i++;
                j--;
            }
        }
        quick_sort(arr, j + 1);
        quick_sort(arr + i, n - i);
    }
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


void Bentley_Ottman_alg(int n, all * c, seg *result, int* result_counter){
    event * heap = malloc(10 * n * n * sizeof (event));
    int size = 0;
    for (int i = 0; i < n; i++){
        ins(heap, &size, (event){c->x1[i], 0, i, -1});;
        ins(heap, &size, (event){c->x2[i], 1, i, -1});
    }

    struct list * head = NULL;
    while (size > 0){
        event curr_ev = pop(heap, &size);
        c->cur_x = curr_ev.x;
        if (curr_ev.type == 0){
            insert(&head, curr_ev.index1, c);
            int next = get_next(head, curr_ev.index1, c);
            int prev = get_prev(head, curr_ev.index1, c);
            double tmp = 0;
            if (next != -1) {
                check_cross(c->x1[curr_ev.index1], c->y1[curr_ev.index1], c->x2[curr_ev.index1], c->y2[curr_ev.index1], c->x1[next], c->y1[next], c->x2[next], c->y2[next], c->indx[curr_ev.index1], c->indx[next], result_counter, result);
                if (get_intersect_x(curr_ev.index1, next, c, &tmp) && tmp > curr_ev.x + epsilon){
                    ins(heap, &size, (event){tmp, 2, curr_ev.index1, next});
                }
            }
            if (prev != -1) {
                check_cross(c->x1[curr_ev.index1], c->y1[curr_ev.index1], c->x2[curr_ev.index1], c->y2[curr_ev.index1], c->x1[prev], c->y1[prev], c->x2[prev], c->y2[prev], c->indx[curr_ev.index1], c->indx[prev], result_counter, result);
                if (get_intersect_x(curr_ev.index1, prev, c, &tmp) && tmp > curr_ev.x + epsilon){
                    ins(heap, &size, (event){tmp, 2, prev, curr_ev.index1});
                }
            }
        }
        if (curr_ev.type == 1){
            int next = get_next(head, curr_ev.index1, c);
            int prev = get_prev(head, curr_ev.index1, c);
            if (next != -1 && prev != -1) {
                double tmp = 0;
                check_cross(c->x1[prev], c->y1[prev], c->x2[prev], c->y2[prev], c->x1[next], c->y1[next], c->x2[next], c->y2[next], c->indx[prev], c->indx[next], result_counter, result);
                if (get_intersect_x(next, prev, c, &tmp) && tmp > curr_ev.x + epsilon){
                    ins(heap, &size, (event){tmp, 2, prev, next});
                }
            }
            head = delete(head, curr_ev.index1, c);
        }
        if (curr_ev.type == 2){
            c->cur_x-=(epsilon * 1000);
            head = delete(head, curr_ev.index1, c);
            head = delete(head, curr_ev.index2, c);
            c->cur_x += (2 * epsilon * 1000);// для небольшого смещения
            insert(&head, curr_ev.index1, c);
            insert(&head, curr_ev.index2, c);
            int next1 = get_next(head, curr_ev.index1, c);
            int prev1 = get_prev(head, curr_ev.index1, c);
            int next2 = get_next(head, curr_ev.index2, c);
            int prev2 = get_prev(head, curr_ev.index2, c);

            double t;
            if (next1 != -1){
                check_cross(c->x1[curr_ev.index1], c->y1[curr_ev.index1], c->x2[curr_ev.index1], c->y2[curr_ev.index1], c->x1[next1], c->y1[next1], c->x2[next1], c->y2[next1], c->indx[curr_ev.index1], c->indx[next1], result_counter, result);
                if (get_intersect_x(curr_ev.index1, next1, c, &t) && t > curr_ev.x + epsilon){
                    ins(heap, &size, (event) {t, 2, curr_ev.index1, next1});
                }
            }
            if (prev1 != -1){
                check_cross(c->x1[curr_ev.index1], c->y1[curr_ev.index1], c->x2[curr_ev.index1], c->y2[curr_ev.index1], c->x1[prev1], c->y1[prev1], c->x2[prev1], c->y2[prev1], c->indx[curr_ev.index1], c->indx[prev1], result_counter, result);
                if (get_intersect_x(curr_ev.index1, prev1, c, &t) && t > curr_ev.x + epsilon){
                    ins(heap, &size, (event) {t, 2, prev1, curr_ev.index1});
                }
            }
            if (next2 != -1){
                check_cross(c->x1[curr_ev.index2], c->y1[curr_ev.index2], c->x2[curr_ev.index2], c->y2[curr_ev.index2], c->x1[next2], c->y1[next2], c->x2[next2], c->y2[next2], c->indx[curr_ev.index2], c->indx[next2], result_counter, result);
                if (get_intersect_x(curr_ev.index2, next2, c, &t) && t > curr_ev.x + epsilon){
                    ins(heap, &size, (event) {t, 2, curr_ev.index2, next2});
                }
            }
            if (prev2 != -1){
                check_cross(c->x1[curr_ev.index2], c->y1[curr_ev.index2], c->x2[curr_ev.index2], c->y2[curr_ev.index2], c->x1[prev2], c->y1[prev2], c->x2[prev2], c->y2[prev2], c->indx[curr_ev.index2], c->indx[prev2], result_counter, result);
                if (get_intersect_x(curr_ev.index2, prev2, c, &t) && t > curr_ev.x + epsilon){
                    ins(heap, &size, (event) {t, 2, prev2, curr_ev.index2});
                }
            }
        }
    }
    clear(head);
    free(heap);
}


int main(void) {
    int tmp_command = -1;
    struct list* head = NULL;
    int n = 0;
    scanf("%d", &n);
    int * indx = (int*)malloc(n * sizeof(int));
    double *x1 = (double *)malloc(n * sizeof(double));
    double *y1 = (double *)malloc(n * sizeof(double));
    double *x2 = (double *)malloc(n * sizeof(double));
    double *y2 = (double *)malloc(n * sizeof(double));
    all c;
    c.indx = indx;
    c.x1 = x1;
    c.y1 = y1;
    c.x2 = x2;
    c.y2 = y2;
    for (int i = 0; i < n; i++){
        scanf("%d %lf %lf %lf %lf", &indx[i], &x1[i], &y1[i], &x2[i], &y2[i]);
        if (x1[i] > x2[i]){
            double tmp_x = x1[i];
            x1[i] = x2[i];
            x2[i] = tmp_x;
            double tmp_y = y1[i];
            y1[i] = y2[i];
            y2[i] = tmp_y;
        }
    }

    seg * answer = malloc(n * n * 10 * sizeof (seg));
    int answer_counter = 0;
    Bentley_Ottman_alg(n, &c, answer, &answer_counter);
    if (answer_counter > 0){
        quick_sort(answer, answer_counter);
        for (int i = 0; i < answer_counter; i++){
            if (i== 0 || answer[i].u != answer[i-1].u || answer[i].v != answer[i-1].v){
                printf("%d %d\n", answer[i].u, answer[i].v);
            }
        }
    }
    free(indx);
    free(x1);
    free(y1);
    free(x2);
    free(y2);
    free(answer);
    clear(head);
    return 0;
}