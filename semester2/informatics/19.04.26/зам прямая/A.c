#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct list{
    int value;
    struct list * left_child;
    struct list * right_child;
};

#define epsilon 1e-12
#define infinity 1e7


void insert(struct list** head, int value){
    struct list* new_list = (struct list*)malloc(sizeof(struct list));
    new_list->value = value;
    new_list ->left_child = NULL;
    new_list ->right_child = NULL;
    if (!*head){
        *head = new_list;
        return;
    }
    struct list * tmp = * head;
    struct list * parent = NULL;

    while(tmp){
        parent = tmp;
        if (value < tmp -> value){
            tmp = tmp -> left_child;
        }
        else{
            if (value > tmp -> value){
                tmp = tmp -> right_child;
            }
            else{
                free(new_list);
                return;
            }
        }
    }
    if (value < parent -> value){
        parent -> left_child = new_list;
    }
    else{
        parent -> right_child = new_list;
    }
    return;
}

void clear(struct list* head){
    if (head == NULL){
        return;
    }
    clear(head -> left_child);
    clear(head -> right_child);
    free(head);
}


void fill_arr(struct list *head, int *arr, int *indx) {
    if (!head){
        return;
    }
    fill_arr(head->left_child, arr, indx);
    arr[(*indx)++] = head->value;
    fill_arr(head->right_child, arr, indx);
}


typedef struct triangle{
    int x[3];
    int y[3];
    int min_y;
    int max_y;
}triangle;

typedef struct edge{
    int to;
    struct edge *next;
}edge;

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


double get_left_x(triangle *t, int y) {
    double best = infinity;
    int first = 1;
    for (int i = 0; i < 3; i++) {
        int x1 = t->x[i], y1 = t->y[i];
        int x2 = t->x[(i + 1) % 3], y2 = t->y[(i + 1) % 3];
        int y_lo = min(y1, y2), y_hi = max(y1, y2);
        if (!(y < y_lo || y > y_hi)) {
            double cand;
            if (y1 == y2){
                cand= min(x1, x2);
            }
            else {
                double t = (double)(y - y1) / (y2 - y1);
                cand = x1 + t * (x2 - x1);
            }
            if (first || cand < best - epsilon){
                best = cand;
                first = 0;
            }
        }
    }
    return best;
}

double get_right_x(triangle *t, int y) {
    double best = - infinity;
    int first = 1;
    for (int i = 0; i < 3; i++) {
        int x1 = t->x[i];
        int y1 = t->y[i];
        int x2 = t->x[(i + 1) % 3];
        int y2 = t->y[(i + 1) % 3];
        int y_lo = min(y1, y2);
        int y_hi = max(y1, y2);
        if (!(y < y_lo || y > y_hi)) {
            double cand;
            if (y1 == y2){
                cand = max(x1, x2);
            }
            else{
                double t = (double)(y - y1) / (y2 - y1);
                cand = x1 + t * (x2 - x1);
            }
            if (first || cand > best + epsilon){
                best = cand;
                first = 0;
            }
        }
    }
    return best;
}


bool left_check(triangle *tri, int i, int j, int y) {
    get_left_x(&tri[i], y);
    get_right_x(&tri[j], y);
    return get_left_x(&tri[i], y) <= get_right_x(&tri[j], y) + epsilon;
}


bool chack_crass(triangle *tri, int i, int j){
    int y_low = max(tri[i].min_y, tri[j].min_y);
    int y_high = min(tri[i].max_y, tri[j].max_y);
    if (y_low > y_high){
        return false;
    }
    if (left_check(tri, i, j, y_low)){
        return true;
    }
    if (left_check(tri, i, j, y_high)){
        return true;
    }
    for (int k = 0; k < 3; ++k){
        int y = tri[i].y[k];
        if (y > y_low && y < y_high && left_check(tri, i, j, y))
            return true;
    }
    for (int k = 0; k < 3; ++k){
        int y = tri[j].y[k];
        if (y > y_low && y < y_high && left_check(tri, i, j, y))
            return true;
    }
    return false;
}


void adding_edge(edge ** mat, int *in_deg, int from, int to){
    edge * nodes = (edge*)malloc(sizeof (edge));
    nodes->to = to;
    nodes -> next = mat[from];
    mat[from] = nodes;
    in_deg[to]++;
}



int main(void) {
    int n;
    scanf("%d", &n);
    triangle *triangles = (triangle *)malloc(n * sizeof(triangle));
    edge **mat = (edge**)calloc(n, sizeof(edge *));
    int *in_deg = (int *)calloc(n, sizeof(int));
    struct list *head = NULL;
    for (int i = 0; i < n; ++i) {
        for (int v = 0; v < 3; ++v)
            scanf("%d %d", &triangles[i].x[v], &triangles[i].y[v]);
        triangles[i].min_y = min(min(triangles[i].y[0], triangles[i].y[1]), triangles[i].y[2]);
        triangles[i].max_y = max(max(triangles[i].y[0], triangles[i].y[1]), triangles[i].y[2]);
        insert(&head, triangles[i].min_y);
        insert(&head, triangles[i].max_y);
    }

    int *events = (int *)malloc(2 * n * sizeof(int));
    int counter_of_events = 0;
    fill_arr(head, events, &counter_of_events);
    clear(head);

    int * active_ev = (int*)malloc(n * sizeof(int));
    int counter_of_active = 0;
    int *new_triangles = (int*)malloc(n * sizeof(int));


    int flag = 1;
    for (int i = 0; i < counter_of_events && flag; i++){
        int y = events[i];
        int new_counter = 0;
        for (int j = 0; j < n; j++){
            if (triangles[j].min_y == y){
                new_triangles[new_counter++] = j;
            }
        }
        for (int j = 0; j < new_counter && flag; j++){
            int tmp = new_triangles[j];
            for (int k = 0; k < counter_of_active; k++){
                int active = active_ev[k];
                bool cur_cross_act = chack_crass(triangles, tmp, active);
                bool cur_cross_tmp = chack_crass(triangles, active, tmp);
                if (cur_cross_tmp && cur_cross_act){
                    flag = 0;
                }
                else{
                    if (cur_cross_act){
                        adding_edge(mat, in_deg, active, tmp);
                    }
                    if (cur_cross_tmp){
                        adding_edge(mat, in_deg, tmp, active);
                    }
                }
            }
            if (flag){
                for (int k =0; k < j; k++){
                    int tmp2 = new_triangles[k];
                    bool cur_cross_act2 = chack_crass(triangles, tmp, tmp2);
                    bool cur_cross_tmp2 = chack_crass(triangles, tmp2, tmp);
                    if (cur_cross_act2 && cur_cross_tmp2){
                        flag = 0;
                    }
                    else{
                        if (cur_cross_act2){
                            adding_edge(mat, in_deg, tmp2, tmp);
                        }
                        if (cur_cross_tmp2){
                            adding_edge(mat, in_deg, tmp, tmp2);
                        }
                    }
                }
            }
        }
        for (int j = 0; j < new_counter; j++) {
            active_ev[counter_of_active++] = new_triangles[j];
        }
        int keep = 0;
        for (int o = 0; o < counter_of_active; o++){
            if (triangles[active_ev[o]].max_y != y){
                active_ev[keep ++] = active_ev[o];
            }
        }
        counter_of_active = keep;
    }

    if (!flag){
        printf("-1");
    }
    else{
        int *queue = (int *) malloc(n * sizeof(int));
        int head1 = 0;
        int tail = 0;
        for (int i = 0; i < n; i++){
            if (in_deg[i] == 0){
                queue[tail++] = i;
            }
        }
        int * final = (int *)malloc(n * sizeof(int));
        int counter_of_final = 0;
        while (head1 < tail){
            int tmp = queue[head1++];
            final[counter_of_final++] = tmp;
            edge * e = mat[tmp];
            while(e){
                int x = e-> to;
                in_deg[x] -= 1;
                if (in_deg[x] == 0){
                    queue[tail++] = x;
                }
                edge * tmp2 = e;
                e = e->next;
                free(tmp2);
            }
        }
        if (counter_of_final < n){
            printf("-1");
        }
        else{
            for (int i = 0; i < counter_of_final; i++){
                printf("%d ", final[i] + 1);
            }
        }
        free(queue);
        free(final);
    }

    free(triangles);
    free(mat);
    free(in_deg);
    free(events);
    free(active_ev);
    free(new_triangles);
    return 0;
}