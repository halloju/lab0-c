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

    element_t *curr, *safe;

    list_for_each_entry_safe(curr, safe, head, list) {
        q_release_element(curr);
    }
    free(head);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head || !s)
        return false;

    element_t *e = malloc(sizeof(element_t));
    if (!e)
        return false;

    e->value = strdup(s);
    if (!e->value) {
        free(e);
        return false;
    }

    list_add(&e->list, head);
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head || !s)
        return false;

    element_t *e = malloc(sizeof(element_t));
    if (!e)
        return false;

    e->value = strdup(s);
    if (!e->value) {
        free(e);
        return false;
    }
    list_add_tail(&e->list, head);
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;

    element_t *entry = list_first_entry(head, element_t, list);
    if (sp && bufsize) {
        strncpy(sp, entry->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }

    list_del_init(&entry->list);
    return entry;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;
    element_t *entry = list_last_entry(head, element_t, list);
    if (sp && bufsize) {
        strncpy(sp, entry->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }
    list_del_init(&entry->list);
    return entry;
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
    if (!head || list_empty(head))
        return false;

    struct list_head *slow = head->next;
    struct list_head *fast = head->next;

    while (fast != head && fast->next != head) {
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
    if (!head || list_empty(head))
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
    if (!head || list_empty(head) || list_is_singular(head))
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
    if (!head || list_empty(head) || k <= 1)
        return;

    if (q_size(head) < k)
        return;

    struct list_head *curr = head->next;

    while (curr != head) {
        // Check if we have k nodes ahead
        struct list_head *grp_end = curr;
        int count = 1;  // count starts at 1 because curr is included

        for (int i = 1; i < k && grp_end != head; i++) {
            grp_end = grp_end->next;
            count++;
        }

        if (count < k || grp_end == head)
            break;

        struct list_head *next_group_start = grp_end->next;
        struct list_head *grp_prev = curr->prev;

        while (curr != next_group_start) {
            struct list_head *next = curr->next;
            list_move(curr, grp_prev);  // Always move after grp_prev
            curr = next;
        }

        // Move to the next group
        curr = next_group_start;
    }
}


/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return;

    // Split the list into two halves
    struct list_head *slow = head->next;
    struct list_head *fast = head->next;

    while (fast != head && fast->next != head) {
        slow = slow->next;
        fast = fast->next->next;
    }
    slow = slow->prev;  // Move slow back to the last node of the first half

    // Create a new list for the second half
    struct list_head second;
    INIT_LIST_HEAD(&second);

    // Split the list at the middle point
    list_cut_position(&second, head, slow);

    // Recursively sort each half
    q_sort(head, descend);
    q_sort(&second, descend);

    // Create a temporary list to store merged results
    struct list_head result;
    INIT_LIST_HEAD(&result);

    // Merge the sorted halves
    while (!list_empty(head) && !list_empty(&second)) {
        const element_t *head_elem = list_entry(head->next, element_t, list);
        const element_t *second_elem = list_entry(second.next, element_t, list);

        int cmp = strcmp(head_elem->value, second_elem->value);
        bool should_take_head = descend ? (cmp >= 0) : (cmp < 0);

        if (should_take_head) {
            list_move_tail(head->next, &result);
        } else {
            list_move_tail(second.next, &result);
        }
    }

    // Attach remaining elements
    if (!list_empty(&second))
        list_splice_tail_init(&second, &result);

    // Move everything back to the original head
    if (!list_empty(head))
        list_splice_tail_init(head, &result);

    // Move everything from result back to head
    list_splice_tail_init(&result, head);
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
