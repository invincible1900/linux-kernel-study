// 测试 list_add 并发

#include <stddef.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define THREADS 100

static int c = 0;

int rets[THREADS];

// #######################################################
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

#define list_for_each_prev(pos, head) \
    for (pos = (head)->prev; pos != (head); pos = pos->prev)


#define container_of(ptr, type, member) ({              \
    void *__mptr = (void *)(ptr);                   \
    ((type *)(__mptr - offsetof(type, member))); })


#define list_entry(ptr, type, member) \
    container_of(ptr, type, member)

LIST_HEAD(head);

// head->n->(n-1)->...->2->1->head (1 最早加入)
// head->next 指向最近加入的
// head->prev 指向最早加入的
void list_add(struct list_head *new, struct list_head *head)
{
#if 1
    struct list_head *prev = head;
    struct list_head *next = head->next;

    new->next = next;
    new->prev = prev;
    next->prev = new;
    prev->next = new;
#else
    head->next->prev = new;
    new->next = head->next;
    new->prev = head;
    head->next = new;
#endif
}

// ####################################################### 

struct my_type{
    struct list_head list;
    char *c;
    int d;

};

void print_list(struct list_head *head){
    struct list_head *pos;
    struct my_type *p;
    if(c != THREADS){
        list_for_each(pos, head){
                p = list_entry(pos, struct my_type, list);
                printf("\tp->d:%d\n", p->d);
        }
        printf("#%d\n", c);
        list_for_each_prev(pos, head){
            p = list_entry(pos, struct my_type, list);
            printf("\tp->d:%d\n", p->d);
        }
    }
}

void *add(void *argv){

        struct my_type * t = (struct my_type *)argv;

        rets[t->d] = 66;
        c += 1;
        sleep(0.5); // 这里sleep 可以增加几率
        list_add(&(t->list), &head);

        pthread_exit(&rets[t->d]);
}

struct my_type t[THREADS];

int main(int argc, char *argv[]){
    int *ret;
    pthread_t id[THREADS];

    for(int i = 0; i < THREADS; i++){
            t[i].c = "Hello";
            t[i].d = i;
            pthread_create(&id[i], NULL, add, &t[i]);
    }

    for(int i = 0; i < THREADS; i++){
            pthread_join(id[i], (void **)&ret);
    }

    for(int i = 0; i<THREADS; i++){
            if(rets[i] != 66){
                    printf("#error: %d, %d\n", i, rets[i]);
                    return 0;
            }
    }

    print_list(&head);
}

