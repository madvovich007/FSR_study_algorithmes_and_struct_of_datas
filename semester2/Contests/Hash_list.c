#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>


#define Size 10


struct list{
    int key;
    double value;
    struct list * next;
};


int ins(struct list** head, int key, double value){
    struct list* curr = *head;
    while (curr){
        if (curr->key == key){
            return 2;
        }
        curr = curr->next;
    }
    struct list* new_list = (struct list*)malloc(sizeof(struct list));
    new_list->value = value;
    new_list->key = key;
    if (!*head){
        new_list -> next = NULL;
        *head = new_list;
        return 0;
    }
    new_list -> next = *head;
    *head = new_list;
    return 0;
}


struct list* search(struct list *head, int key){
    while (head){
        if (head->key == key){
            return head;
        }
        head = head->next;
    }
    return NULL;
}


int pop(struct list** head, int key){
    if (*head == NULL) {
        return 0;
    }
    struct list *curr = *head;
    struct list *prev = NULL;
    while (curr && curr->key != key){
        prev = curr;
        curr = curr->next;
    }
    if (!curr){
        return 0;
    }
    if (prev){
        prev->next = curr->next;
    }
    else{
        *head = curr->next;
    }
    free(curr);
    return 1;
}


void clear(struct list ** head){
    struct list *curr = *head;
    while (curr){
        struct list *next = curr->next;
        free(curr);
        curr = next;
    }
    *head = NULL;
}


typedef struct hash_table{
    int size;
    struct list **buckets;
}hash_table;


int hash_func(int key){
    double tmp = key * M_PI;
    tmp -= (int)tmp;
    return (int)(tmp * Size);
}


void ht_init(hash_table *hash_table, int size){
    hash_table->size = size;
    hash_table->buckets =(struct list**)calloc(size, sizeof(struct list*));
}


int hash_table_insert(hash_table *hash_table, int key, double value){
    int index = hash_func(key);
    return ins(&hash_table->buckets[index], key, value);
}


int hash_table_search(hash_table * hash_table, int key, double * value){
    int index = hash_func(key);
    struct list *node = search(hash_table->buckets[index], key);
    if (!node){
        return 0;
    }
    *value = node->value;
    return 1;
}


int hash_table_delete(hash_table *hash_table, int key){
    int index = hash_func(key);
    return pop(&hash_table->buckets[index], key);
}


void hash_table_clear(hash_table *hash_table){
    for (int i = 0; i < hash_table->size; i++){
        clear(&hash_table->buckets[i]);
    }
}


int main(void){
    hash_table hash_table;
    ht_init(&hash_table, Size);
    int tmp_command = -1;
    int key = 0;
    double value = 0.0;
    while (tmp_command != 0){
        if (scanf("%d", &tmp_command) == 1) {
            switch (tmp_command) {
                case 1:
                    scanf(" %d %lf", &key, &value);
                    printf("%d\n", hash_table_insert(&hash_table, key, value));
                    break;
                case 2:
                    scanf(" %d", &key);
                    if (hash_table_search(&hash_table, key, &value)) {
                        printf("%lf\n", value);
                    } else {
                        printf("Not found\n");
                    }
                    break;
                case 3:
                    scanf(" %d", &key);
                    if (hash_table_delete(&hash_table, key)) {
                        printf("0\n");
                    } else {
                        printf("Not found\n");
                    }
                    break;
                case 4:
                    hash_table_clear(&hash_table);
                    printf("0\n");
                    break;
                case 0:
                    break;
                default:
                    break;
            }
        }
    }
    for (int i = 0; i < hash_table.size; i++){
        clear(&hash_table.buckets[i]);
    }
    free(hash_table.buckets);
    return 0;
}