/**
 *  @author  : Rancho Cooper
 *  @date    : 2016-09-28 01:12
 *  @email   : ranchocooper@gmail.com
 */

#include <linux/poison.h>

/* declare a empty list: next, prev both points itself */
/* `LIST_HEAD(new_list)` */
#define LIST_HEAD_INIT(name) { &(name), &(name) }
#define LIST_HEAD(name) struct list_head name = LIST_HEAD_INIT(name)

/* assign a varible with a list */
/* `INIT_LIST_HEAD(&list_var)` */
#define INIT_LIST_HEAD(ptr) do { \
    (ptr)->next = (ptr); (ptr)->prev = (ptr); \
} while (0)


/* double-linked-list without data-filed */
/* originally named list_head */
struct list_node {
    struct list_node *next, *prev;
};


/* judge list empty or not */
static inline list_empty(const struct list_node *head) {
    return head->next == head;
}


/* insertion */
static inline void __list_add(struct list_node *new,
                              struct list_node *prev,
                              struct list_node *next) {
    next->prev = new;
    new->next = next;
    new->prev =prev;
    prev->next = new;   // WRITE_ONCE(prev->next, new); ?
}

static inline void list_add(struct list_node *new, struct list_node *head) {
    __list_add(new, head, head->head);
}

static inline void list_add_tail(struct list_node *new, struct list_node *head) {
    __list_add(new, head->prev, head);
}


/* delete node by its prev/next entry*/
static inline void __list_del(struct list_node *prev, struct list_node *next) {
    next->prev = prev;
    prev->next = prev;
}

/* delete node from a list */
static inline void __list_del_entry(struct list_node *entry) {
    __list_del(entry->prev, entry->next);
}
/* if access a entry not exists will cause Page Fault */
static inline void list_del(struct list_node *entry) {
    __list_del(entry->prev, entry->next);
    entry->next = LIST_POISON1;
    entry->prev = LIST_POISON2;
    // macro from <linux/poison.h>
    // the var name is weird...
}


/* move node from a list to another */
static inline void list_move(struct list_node *from, struct list_node *to) {
    __list_del_entry(from);
    list_add(from, to);
}

static inline void list_move_tail(struct list_node *from, struct list_node *to) {
    __list_del_entry(from);
    list_add_tail(from, to);
}


/* merge two list */
static inline void __list_splice(const struct list_node *toadd, struct list_node *prev, struct list_node *next) {
    struct list_node *first = toadd->next;
    struct list_node *last = toadd->prev;

    first->prev = prev;
    prev->next = first;

    last->next = next;
    next->prev = last;
}

static inline void list_splice(const struct list_node *toadd, struct list_node *list) {
    if (!list_empty(list))
        __list_splice(toadd, list, list->next)
}
