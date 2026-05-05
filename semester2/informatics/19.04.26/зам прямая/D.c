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

int extract(struct list* head, int *arr){
    struct list * stack[100000];
    int top = 0;
    int index = 0;
    struct list * tmp = head;
    while(tmp || top > 0){
        while (tmp){
            stack[top++] = (struct list *)tmp;
            tmp = tmp->left_child;
        }
        tmp = stack[--top];
        arr[index++] = tmp->value;
        tmp = tmp->right_child;
    }
    return index;
}


int find_index(int * arr, int n, int val) {
    int low = 0, high = n - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        if (arr[mid] == val){
            return mid;
        }
        else if (arr[mid] < val){
            low = mid + 1;
        }
        else{
            high = mid - 1;
        }
    }
    return -1;
}


int main(void){
    int n = 0;
    scanf("%d", &n);
    struct list * head = NULL;
    insert(&head, 0);
    insert(&head, 86400);
    int * open = (int *)malloc(n * sizeof(int));
    int * close = (int *)malloc(n * sizeof(int));
    int * type = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++){
        int h, m, s, h2, m2, s2;
        scanf("%d %d %d %d %d %d", &h, &m, &s, &h2, &m2, &s2);
        open[i] = h * 3600 + m * 60 + s;
        close[i] = h2 * 3600 + m2 * 60 + s2;
        if (h2 * 3600 + m2 * 60 + s2 == h * 3600 + m * 60 + s){
            type[i] = 2;
        }
        else if(h * 3600 + m * 60 + s < h2 * 3600 + m2 * 60 + s2){
            type[i] = 0;
            insert(&head, h * 3600 + m * 60 + s);
            insert(&head, h2 * 3600 + m2 * 60 + s2);
        }
        else{
            type[i] = 1;
            insert(&head, h * 3600 + m * 60 + s);
            insert(&head, h2 * 3600 + m2 * 60 + s2);
        }
    }


    int *times = (int *)malloc(100000 * sizeof(int));
    int m = extract(head, times);
    clear(head);

    int *delta = (int *)calloc(m, sizeof(int));
    int i0 = find_index(times, m, 0);
    int i_last = find_index(times, m, 86400);

    for (int i = 0; i < n; ++i){
        if (type[i] == 0) {
            int i1 = find_index(times, m, open[i]);
            int i2 = find_index(times, m, close[i]);
            delta[i1]++;
            delta[i2]--;
        }
        else if (type[i] == 1){
            int idx_open = find_index(times, m, open[i]);
            int idx_close = find_index(times, m, close[i]);
            delta[i0]++;
            delta[idx_close]--;
            delta[idx_open]++;
            delta[i_last]--;
        }
        else{
            delta[i0]++;
            delta[i_last]--;
        }
    }

    int total = 0;
    int active = 0;
    for (int i = 0; i < m - 1; ++i){
        active += delta[i];
        if (active == n){
            total += (times[i + 1] - times[i]);
        }
    }
    printf("%d", total);
    free(open);
    free(close);
    free(type);
    free(times);
    free(delta);
    return 0;
}



