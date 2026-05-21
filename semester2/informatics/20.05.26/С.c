#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define maxi_count 26

struct list{
    char value;
    int end;
    struct list * childrens[maxi_count];
};

int char_to_index(char c){
    if (c >= 'a' && c <= 'z'){
        return c - 'a';
    }
    return -1;
}

char index_to_char(int i){
    return 'a' + (i);
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

int len(char* arr){
    int c = 0;
    while (arr[c] != '\0'){
        c++;
    }
    return c;
}

void copying(char * a, char * b){
    int i = 0;
    while (a[i] != '\0'){
        b[i] = a[i];
        i++;
    }
    b[i] = '\0';
    return;
}

int collect_bigger_word(struct list * head, int d, char * curr, int k, char ** out, int * count){
    if (d >= k + 1){
        if (head->end){
            int len1 = len(curr);
            char * tmp = (char *)malloc((len1 + 1) * sizeof(char));
            copying(curr, tmp);
            out[*count] = tmp;
            (*count)++;
            return 1;
        }
        for (int i = 0; i < maxi_count; i++){
            if (head->childrens[i] != NULL){
                curr[d] = index_to_char(i);
                curr[d + 1] = '\0';
                int flag = collect_bigger_word(head->childrens[i], d + 1, curr, k, out, count);
                if (flag){
                    return 1;
                }
            }
        }
        return 0;
    }
    else{
        for(int i = 0; i < maxi_count; i++){
            if (head->childrens[i] != NULL){
                curr[d] = index_to_char(i);
                curr[d + 1] = '\0';
                int falg = collect_bigger_word(head->childrens[i], d + 1, curr, k, out, count);
            }
        }
    }
    return 0;
}

int main(void){
    int n = 0;
    int k = 0;
    scanf("%d %d", &n, &k);
    char ** words = (char **) malloc(n * sizeof(char * ));
    struct list * head = create('#');
    for (int i = 0; i < n ; i++){
        char tmp[300];
        scanf("%s", tmp);
        int len1 = len(tmp);
        char *copy = (char *)malloc((len1 + 1) * sizeof(char));
        copying(tmp, copy);
        words[i] = copy;
        insert(head, tmp);
    }

    char ** small = (char **)malloc(n * sizeof (char *));
    int counter_small = 0;
    for (int i = 0; i < n; i++){
        if (len(words[i]) <= k){
            small[counter_small] = words[i];
            counter_small++;
        }
    }

    char ** big = (char **)malloc(n * sizeof(char *));
    int counter_big = 0;
    char curr[1000];
    for (int i = 0; i < maxi_count; i++){
        if (head->childrens[i] != NULL){
            curr[0] = index_to_char(i);
            curr[1] = '\0';
            collect_bigger_word(head->childrens[i], 1, curr, k, big, &counter_big);
        }
    }
    printf("%d\n", counter_big + counter_small);
    for (int i = 0; i < counter_small; i++){
        printf("%s\n", small[i]);
    }
    for (int i = 0; i < counter_big; i++){
        printf("%s\n", big[i]);
    }

    for (int i = 0; i < n; i++){
        free(words[i]);
    }
    for (int i = 0; i < counter_big; i++){
        free(big[i]);
    }
    free(words);
    free(big);
    free(small);
    clear(head);
    return 0;
}