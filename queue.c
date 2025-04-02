#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *head = malloc(sizeof(struct list_head));
    if (head)
        INIT_LIST_HEAD(head);
    return head;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    if (!head)
        return;

    struct list_head *curr, *safe;

    list_for_each_safe(curr, safe, head) {
        element_t *e = list_entry(curr, element_t, list);
        list_del(curr);
        if (e->value)
            free(e->value);
        free(e);
    }
    free(head);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head || !s)
        return false;

    element_t *e = malloc(sizeof(element_t));

    e->value = strdup(s);
    list_add(&e->list, head);
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head || !s)
        return false;

    element_t *e = malloc(sizeof(element_t));

    e->value = strdup(s);
    list_add_tail(&e->list, head);
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || !sp || bufsize == 0)
        return NULL;

    struct list_head *li = head->next;
    if (li == head)
        return NULL;

    element_t *e = list_entry(li, element_t, list);
    strncpy(sp, e->value,
            bufsize); /*Not sure why we need this, but it is in the argu*/
    list_del(li);
    return e;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || !sp || bufsize == 0)
        return NULL;

    struct list_head *li = head->prev;
    if (li == head)
        return NULL;

    element_t *e = list_entry(li, element_t, list);
    strncpy(sp, e->value,
            bufsize); /*Not sure why we need this, but it is in the argu*/
    list_del(li);
    return e;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head)
        return 0;

    int len = 0;
    struct list_head *li;

    list_for_each(li, head)
        len++;
    return len;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    if (!head)
        return false;

    struct list_head *slow = head;
    struct list_head *fast = head;

    while (fast->next != head && fast->next->next != head) {
        slow = slow->next;
        fast = fast->next->next;
    }

    if (slow != head) {
        element_t *e = list_entry(slow, element_t, list);
        list_del(slow);
        free(e->value);
        free(e);
        return true;
    } else {
        return false;
    }
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    if (!head)
        return false;

    struct list_head *curr;
    struct list_head *prev = head;

    while (prev->next != head) {
        curr = prev->next;
        element_t *e = list_entry(curr, element_t, list);
        bool dup = false;

        while (curr->next != head &&
               strcmp(e->value,
                      list_entry(curr->next, element_t, list)->value) == 0) {
            struct list_head *tmp = curr->next;
            list_del(tmp);
            free(list_entry(tmp, element_t, list)->value);
            free(list_entry(tmp, element_t, list));
            dup = true;
        }

        if (dup) {
            list_del(curr);
            free(e->value);
            free(e);
        } else {
            prev = curr;
        }
    }
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head) {}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend) {}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
