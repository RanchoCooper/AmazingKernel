/**
 *  @author  : Rancho Cooper
 *  @date    : 2016-09-28 01:12
 *  @email   : ranchocooper@gmail.com
 *
 *  doubly linked list implementation
 */

#ifndef _LINUX_LIST_H
#define _LINUX_LIST_H

#include <linux/poison.h>
#include <linux/stddef.h>
#include <linux.kernel.h>

/**
 *  @note: declare a empty list: next prev both points itself
 *  @usage: LIST_HEAD(new_list)
 */
#define LIST_HEAD_INIT(name) { &(name), &(name) }
#define LIST_HEAD(name) struct list_head name = LIST_HEAD_INIT(name)


/**
 *  @note: assign a list to a var
 *  @usage: INIT_LIST_HEAD(&list_var)
 */
#define INIT_LIST_HEAD(ptr) do { \
    (ptr)->next = (ptr); (ptr)->prev = (ptr); \
} while (0)
/**
 *  @also
 */
static inline void INIT_LIST_HEAD(struct list_node *list) {
    WRITE_ONCE(list->next, list);
    list->prev = list;
}

struct list_node {
    struct list_node *next, *prev;
};


/**
 *  Insert a new entry between two known consecutive entries
 *  This is only for internal list manipulation where we know
 *  the prev/next entries already
 */
static inline void __list_add(struct list_node *new,
                              struct list_node *prev,
                              struct list_node *next) {
    next->prev = new;
    new->next = next;
    new->prev =prev;
    WRITE_ONCE(prev->next, new);
}

/**
 *  list_add - add a new entry
 *  @new: new entry to be added
 *  @head: list head to add it after
 */
static inline void list_add(struct list_node *new, struct list_node *head) {
    __list_add(new, head, head->next);
}

/**
 *  list_add_tail - add a new entry to tail
 *  @note: useful for implementing queue!
 */
static inline void list_add_tail(struct list_node *new, struct list_node *head) {
    __list_add(new, head->prev, head);
}


/**
 *  Delete a list entry by making the prev/next entries point to
 *  each other.
 */
static inline void __list_del(struct list_node *prev, struct list_node *next) {
    next->prev = prev;
    WRITE_ONCE(prev->next, next);
}

/**
 *  list_del - deletes entry from list
 *  @entry: the element to delete
 */
static inline void __list_del_entry(struct list_node *entry) {
    __list_del(entry->prev, entry->next);
}

/**
 *  if access a entry not exists will cause Page Fault
 *  macro from <linux/poison.h> with weird name...
 */
static inline void list_del(struct list_node *entry) {
    __list_del(entry->prev, entry->next);
    entry->next = LIST_POISON1;
    entry->prev = LIST_POISON2;
}

/**
 *  list_del_init - delete entry from list and reinitialize it
 *  @entry: the element to delete from the list
 */
static inline void list_del_init(struct list_node *entry) {
    __list_del_entry(entry);
    INIT_LIST_HEAD(entry)
}

/**
 *  list_move - delete from one list and add as another's head
 *  @to  : the preceding one
 */
static inline void list_move(struct list_node *from, struct list_node *to) {
    __list_del_entry(from);
    list_add(from, to);
}

/**
 *  list_move_tail - delete from one list and add as another's tail
 *  @to: the following one
 */
static inline void list_move_tail(struct list_node *from, struct list_node *to) {
    __list_del_entry(from);
    list_add_tail(from, to);
}


/**
 *  list_empty - tests whether a list is empty
 */
static inline list_empty(const struct list_node *head) {
    return head->next == head;
}

/**
 *  list_empty_careful - checks no other CPU be in the process of modifying either member (prev/next)
 */
static inline int list_empty_careful(const struct list_node *head) {
    struct list_node *next = head->next;
    return (next == head) && (next == head->prev);
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

/**
 *  list_splice - join tow lists, designed for stacks
 *  @list: the new list to add
 *  @head: the place to add it in the first list
 */
static inline void list_splice(const struct list_node *toadd, struct list_node *list) {
    if (!list_empty(list))
        __list_splice(toadd, list, list->next)
}

/**
 *  list_splice_tail - join two lists, each list being a queue
 */
static inline void list_splice_tail(const struct list_node *toadd, struct list_node *list) {
    if (!list_empty(list))
        __list_splice(toadd, list->prev, list);
}

/**
 *  initial for reinitialise the emptied list
 */
static inline void list_splice_init(struct list_node *toadd
                                    struct list_node *list) {
    if (!list_empty(list)) {
        __list_splice(toadd, list, list->next);
        INIT_LIST_HEAD(list);
    }
}

static inline void list_splice_tail_init(struct list_node *toadd,
                                         struct list_node *list) {
    if (!list_empty(list)) {
        __list_splice(list, list->prev, list);
        INIT_LIST_HEAD(list);
    }
}

/**
 *  list_entry - get the struct for this entry
 *  @ptr:    the &struct list_node pointer
 *  @type:   the type of the struct this is embedded in
 *  @member: the name of the list_head within the struct
 */
#define list_entry(ptr, type, member)           \
    container_of(ptr, type, member)

#define list_first_entry(ptr, type, member)     \
    container_of((ptr)->next, type, member)

#define list_last_entry(ptr, type, member)      \
    container_of((ptr)->prev, type, member)

/* traversal */

/**
 *  list_for_each - iterate over a list
 *  @pos:   the &struct list_node to use as a loop cursor
 *  @head:  the head for your list
 */
#define list_for_each(pos, head)    \
    for (pos = (head)->next; pos != (head); pos = pos->next)

#define list_for_each_prev(pos, head)   \
    for (pos = (head)->prev; pos != (head); pos = pos -> prev)

#define list_for_each_safe(pos, pre, head)  \
    for (pos = (head)->next, pre = pos->next; pos != (head);    \
         pos = pre, pre = pos->next)

#define list_for_each_prev_save(pos, pre, head)     \
    for (pos = (head)->prev, pre = pos->prev; pos != (head);    \
         pos = pre, pre = pos->prev)

