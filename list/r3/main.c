#include <stddef.h>
#include <stdio.h>

// linux-5.4/include/linux/list.h
#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
    struct list_head name = LIST_HEAD_INIT(name)

struct list_head {
    struct list_head *next;
    struct list_head *prev;
};

#define list_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); pos = pos->next)

#define container_of(ptr, type, member) ({              \
    void *__mptr = (void *)(ptr);                   \
    ((type *)(__mptr - offsetof(type, member))); })


#define list_entry(ptr, type, member) \
    container_of(ptr, type, member)


// head->n->(n-1)->...->2->1->head (1 最早加入)
// head->next 指向最近加入的
// head->prev 指向最早加入的
void list_add(struct list_head *new, struct list_head *head)
{
#if 1
    struct list_head *prev = head;
    struct list_head *next = head->next;

    next->prev = new;
    new->next = next;
    new->prev = prev;
    prev->next = new;
#else
    head->next->prev = new;
    new->next = head->next;
    new->prev = head;
    head->next = new;
#endif
}

struct my_type{
    struct list_head list;
    char *c;
    int d;

};


void print_list(struct list_head *head){
    struct list_head *pos;
    struct my_type *p;

    printf("head: %p\n", head);
    printf("\thead->next: %p\n", head->next);
    printf("\thead->prev: %p\n", head->prev);

    list_for_each(pos, head){
            printf("pos: %p\n", pos);
            printf("\tpos->next: %p\n", pos->next);
            printf("\tpos->prev: %p\n", pos->prev);
            p = list_entry(pos, struct my_type, list);
            printf("\tp->c:%s\n", p->c);
            printf("\tp->d:%d\n", p->d);
    
    }

}

LIST_HEAD(head);
struct my_type t[3];

int main(int argc, char *argv[]){

    for(int i = 0; i < 3; i++){
        t[i].c = "Hello";
        t[i].d = i;
        list_add(&(t[i].list), &head);
    }

    print_list(&head);

}
