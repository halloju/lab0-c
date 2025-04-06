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
    if (!head)
        return;

    struct list_head *curr = head->next;
    while (curr != head && curr->next != head) {
        struct list_head *first = curr;
        struct list_head *second = curr->next;

        first->next = second->next;
        second->next = first;
        second->prev = first->prev;
        second->prev->next = second;
        first->prev = second;
        first->next->prev = first;

        curr = first->next;
    }
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head)
        return;

    struct list_head *curr = head->next;
    struct list_head *prev = head;
    struct list_head *next = NULL;
    head->next = head->prev;
    head->prev = curr;

    while (curr != head) {
        next = curr->next;
        curr->next = prev;
        curr->prev = next;
        prev = curr;
        curr = next;
    }
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
    if (!head || k <= 1)
        return;
    struct list_head *curr = head->next;

    while (curr != head) {
        // Check if we have k nodes ahead
        struct list_head *check = curr;
        int count = 0;
        for (int i = 0; i < k; i++) {
            if (check == head)
                break;
            check = check->next;
            count++;
        }

        if (count < k)
            break;

        struct list_head *group_start = curr;
        struct list_head *next_group_start = curr;

        for (int i = 0; i < k; i++) {
            next_group_start = next_group_start->next;
        }

        // Reverse the nodes in the group
        struct list_head *prev = next_group_start;
        for (int i = 0; i < k; i++) {
            struct list_head *tmp = curr->next;
            curr->next = prev;
            prev->prev = curr;
            prev = curr;
            curr = tmp;
        }

        group_start->next = next_group_start;
        next_group_start->prev = group_start;

        // Move to the next group
        curr = next_group_start;
    }
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return;

    // Use insertion sort algorithm for the linked list
    struct list_head *curr = head->next->next;  // Start with the second element

    while (curr != head) {
        struct list_head *next = curr->next;  // Save next before we move curr
        const element_t *curr_elem = list_entry(curr, element_t, list);
        struct list_head *pos = curr->prev;
        bool inserted = false;

        // Find position where current element should be inserted
        while (pos != head) {
            const element_t *pos_elem = list_entry(pos, element_t, list);
            int cmp = strcmp(curr_elem->value, pos_elem->value);

            // For descending order: larger values come first
            // For ascending order: smaller values come first
            if ((descend && cmp < 0) || (!descend && cmp > 0)) {
                // Insert after this position
                if (pos != curr->prev) {
                    list_move(curr, pos);  // Remove curr and add after pos
                    inserted = true;
                }
                break;
            }
            pos = pos->prev;
        }

        // If we reached the head and still need to insert at the beginning
        if (!inserted && pos == head && curr->prev != head) {
            list_move(curr, head);  // Move to the beginning of the list
        }

        curr = next;  // Move to the next element
    }
}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    if (!head || list_empty(head))
        return 0;

    struct list_head *curr = head->prev;

    // Always keep the last node
    const element_t *last_kept = list_entry(curr, element_t, list);
    curr = curr->prev;

    while (curr != head) {
        element_t *curr_elem = list_entry(curr, element_t, list);
        struct list_head *prev = curr->prev;

        if (strcmp(curr_elem->value, last_kept->value) < 0) {
            // Keep it
            last_kept = curr_elem;
        } else {
            // Remove it
            list_del(curr);
            free(curr_elem->value);
            free(curr_elem);
        }

        curr = prev;
    }

    return q_size(head);
}


/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    if (!head || list_empty(head))
        return 0;
    struct list_head *curr = head->prev;
    // Always keep the last node
    const element_t *last_kept = list_entry(curr, element_t, list);
    curr = curr->prev;
    while (curr != head) {
        element_t *curr_elem = list_entry(curr, element_t, list);
        struct list_head *prev = curr->prev;

        if (strcmp(curr_elem->value, last_kept->value) > 0) {
            // Keep it
            last_kept = curr_elem;
        } else {
            // Remove it
            list_del(curr);
            free(curr_elem->value);
            free(curr_elem);
        }

        curr = prev;
    }
    return q_size(head);
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    if (!head || list_empty(head))
        return 0;

    struct list_head *curr = head->next;
    queue_contex_t *first_ctx = list_entry(curr, queue_contex_t, chain);
    struct list_head *first_queue = first_ctx->q;
    curr = curr->next;

    while (curr != head) {
        queue_contex_t *next_ctx = list_entry(curr, queue_contex_t, chain);
        struct list_head *next_queue = next_ctx->q;

        if (!next_queue || list_empty(next_queue)) {
            curr = curr->next;
            continue;
        }

        list_splice_tail_init(next_queue, first_queue);
        q_sort(first_queue, descend);
        first_ctx->size += next_ctx->size;
        next_ctx->size = 0;


        curr = curr->next;
    }
    return first_ctx->size;
}
