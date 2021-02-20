#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"
#include "strnatcmp.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (!q)
        return NULL;

    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (!q)
        return;

    while (q_remove_head(q, NULL, 0))
        ;

    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    if (!q)
        return false;

    list_ele_t *newh = (list_ele_t *) malloc(sizeof(list_ele_t));
    if (!newh) {
        return false;
    }

    int len = strlen(s);
    newh->value = (char *) malloc((len + 1) * sizeof(char));
    if (!newh->value) {
        free(newh);
        return false;
    }
    strncpy(newh->value, s, len + 1);

    newh->next = q->head;
    if (!q->head)
        q->tail = newh;
    q->head = newh;
    q->size += 1;
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    if (!q)
        return false;

    list_ele_t *newh = (list_ele_t *) malloc(sizeof(list_ele_t));
    if (!newh) {
        return false;
    }

    int len = strlen(s);
    newh->value = (char *) malloc((len + 1) * sizeof(char));
    if (!newh->value) {
        free(newh);
        return false;
    }
    strncpy(newh->value, s, len + 1);
    newh->next = NULL;

    if (!q->head) {
        q->head = newh;
    } else {
        q->tail->next = newh;
    }
    q->tail = newh;
    q->size += 1;
    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q || !q->head)
        return false;

    list_ele_t *del = q->head;
    q->head = q->head->next;
    q->size -= 1;
    if (!q->head)
        q->tail = NULL;

    if (sp) {
        size_t len = strlen(del->value);
        size_t maxlen = len + 1 > bufsize ? bufsize : len + 1;
        strncpy(sp, del->value, maxlen - 1);
        sp[maxlen - 1] = '\0';
    }
    free(del->value);
    free(del);

    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (!q)
        return 0;
    return q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (!q || !q->head || !q->head->next)
        return;

    list_ele_t *node = q->head->next;
    list_ele_t *prev = q->head;
    list_ele_t *next;
    while (node->next) {
        next = node->next;
        node->next = prev;
        prev = node;
        node = next;
    }

    q->tail = q->head;
    q->tail->next = NULL;
    q->head = node;
    q->head->next = prev;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    if (!q || !q->head || q->size == 1)
        return;

    list_ele_t *current = q->head;
    list_ele_t *select;
    list_ele_t *min;
    char *tmp;

    while (current->next) {
        select = current->next;
        min = current;
        while (select) {
            if (strnatcasecmp(min->value, select->value) == 1) {
                min = select;
            }
            select = select->next;
        }
        tmp = min->value;
        min->value = current->value;
        current->value = tmp;
        current = current->next;
    }
}

/*
list *sort(list *start) {
    if (!start || !start->next)
        return start;
    list *left = start;
    list *right = left->next;
    LL0;

    left = sort(left);
    right = sort(right);

    for (list *merge = NULL; left || right; ) {
        if (!right || (left && left->data < right->data)) {
            if (!merge) {
                start = merge = left;
            } else {
                merge->next = left;
                merge = merge->next;
            }
            left = left->next;
        } else {
            if (!merge) {
                start = merge = right;
            } else {
                merge->next = right;
                merge = merge->next;
            }
            right = right->next;
        }
    }
    return start;
}
*/
