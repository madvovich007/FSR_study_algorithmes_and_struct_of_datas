#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define maxi_count 63

struct list{
    char value;
    int end;
    struct list * childrens[maxi_count];
};

int char_to_index(char c){
    if (c >= '0' && c <= '9'){
        return c - '0';
    }
    if (c >= 'A' && c <= 'Z'){
        return c - 'A' + 10;
    }
    if (c == '_'){
        return 36;
    }
    if (c >= 'a' && c <= 'z'){
        return c - 'a' + 37;
    }
    return -1;
}

char index_to_char(int i){
    if (i < 10){
        return '0' + i;
    }
    if (i < 36){
        return 'A' + (i - 10);
    }
    if (i == 36){
        return '_';
    }
    return 'a' + (i - 37);
}

struct list * create(char c){
    struct list * head = (struct list *) malloc(1 * sizeof(struct list));
    head->value = c;
    head->end = 0;
    for (int i = 0; i < maxi_count; i++){
     head->childrens[i] = NULL;
    }
    return head;
}

void insert(struct list * head, char * word){
    struct list * cur = head;
    for (int i = 0; word[i] != '\0'; i++){
        int index = char_to_index(word[i]);
        if (!cur->childrens[index]){
            cur->childrens[index] = create(word[i]);
        }
        cur = cur->childrens[index];
    }
    cur->end = 1;
}

int search(struct list * head, char * word) {
    struct list * cur = head;
    for (int i = 0; word[i] != '\0'; i++){
        int index = char_to_index(word[i]);
        if (!cur->childrens[index]) {
            return 0;
        }
        cur = cur->childrens[index];
    }
    return cur->end;
}

void printf_subtree(struct list *node, char *arr, int depth, int *first){
    for (int i = 0; i < maxi_count; i++){
        if (node->childrens[i]) {
            struct list *child = node->childrens[i];
            arr[depth] = child->value;
            arr[depth + 1] = '\0';
            if (!(*first)){
                printf(" ");
            }
            printf("%s", arr);
            *first = 0;
            printf_subtree(child, arr, depth + 1, first);
        }
    }
}


void printf_all_prefixes(struct list *head){
    char * arr = malloc(10000 * sizeof(char));
    int first = 1;
    for (int i = 0; i < maxi_count; i++){
        if (head->childrens[i]) {
            struct list *child = head->childrens[i];
            arr[0] = child->value;
            arr[1] = '\0';
            if (!first){
                printf(" ");
            }
            printf("%s", arr);
            first = 0;
            printf_subtree(child, arr, 1, &first);
        }
    }
    printf("\n");
    free(arr);
    return;
}

int delete_helper(struct list **node, char *word) {
    if (*node == NULL){
        return 0;
    }
    if (*word == '\0'){
        if (!(*node)->end){
            return 0;
        }
        (*node)->end = 0;
        for (int i = 0; i < maxi_count; i++) {
            if ((*node)->childrens[i]){
                return 1;
            }
        }
        free(*node);
        *node = NULL;
        return 1;
    }
    int index = char_to_index(*word);
    int found = delete_helper(&((*node)->childrens[index]), word + 1);
    if (found){
        if (!(*node)->end){
            int has_child = 0;
            int flag = 1;
            for (int i = 0; i < maxi_count && flag; i++) {
                if ((*node)->childrens[i]) {
                    has_child = 1;
                    flag = 0;
                }
            }
            if (!has_child){
                free(*node);
                *node = NULL;
            }
        }
        return 1;
    }
    return 0;
}


int delete(struct list *root, char *word){
    if (*word == '\0'){
        return 0;
    }
    int index = char_to_index(*word);
    if (delete_helper(&root->childrens[index], word + 1)){
        return 1;
    }
    return 0;
}

void clear(struct list* head){
    if (head == NULL){
        return;
    }
    for (int i = 0; i < maxi_count; i++){
        clear(head->childrens[i]);
    }
    free(head);
}


int main(void){
    struct list *head = create('#');
    int tmp_command;
    char word[100];
    while (scanf("%d", &tmp_command) == 1 && tmp_command != 0){
        switch (tmp_command){
            case 1:
                scanf("%s", word);
                insert(head, word);
                printf("1\n");
                break;
            case 2:
                scanf("%s", word);
                printf("%d\n", search(head, word));
                break;
            case 3:
                scanf("%s", word);
                printf("%d\n", delete(head, word));
                break;
            case 4:
                printf_all_prefixes(head);
                break;
            default:
                break;
        }
    }
    clear(head);
    return 0;
}