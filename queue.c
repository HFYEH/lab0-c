#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (q == NULL)
        return NULL;

    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (q == NULL)
        return;

    list_ele_t *del = q->head;
    while (del != NULL) {
        q->head = del->next;
        q->size -= 1;
        if (q->head == NULL) {
            q->tail = NULL;
        }

        free(del->value);
        free(del);
        del = q->head;
    }
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
    if (q == NULL)
        return false;

    list_ele_t *newh = (list_ele_t *) malloc(sizeof(list_ele_t));
    if (newh == NULL) {
        return false;
    }

    int len = strlen(s);
    newh->value = (char *) malloc((len + 1) * sizeof(char));
    if (newh->value == NULL) {
        free(newh);
        return false;
    }
    strncpy(newh->value, s, len + 1);

    newh->next = q->head;
    if (q->head == NULL)
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
    if (q == NULL)
        return false;

    list_ele_t *newh = (list_ele_t *) malloc(sizeof(list_ele_t));
    if (newh == NULL) {
        return false;
    }

    int len = strlen(s);
    newh->value = (char *) malloc((len + 1) * sizeof(char));
    if (newh->value == NULL) {
        free(newh);
        return false;
    }
    strncpy(newh->value, s, len + 1);
    newh->next = NULL;

    if (q->head == NULL) {
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
    if (q == NULL || q->head == NULL)
        return false;

    list_ele_t *del = q->head;
    q->head = q->head->next;
    q->size -= 1;
    if (q->head == NULL)
        q->tail = NULL;

    if (sp != NULL) {
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
    if (q == NULL)
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
    if (q == NULL || q->head == NULL)
        return;

    list_ele_t *current = q->tail;
    list_ele_t *prev;
    while (current != q->head) {
        prev = q->head;
        while (current != prev->next) {
            prev = prev->next;
        }
        current->next = prev;
        current = prev;
    }
    current->next = NULL;
    q->head = q->tail;
    q->tail = current;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    if (q == NULL || q->head == NULL || q->size == 1)
        return;

    list_ele_t *current = q->head;
    list_ele_t *compare;
    char *tmp;

    while (current->next != NULL) {
        compare = current->next;
        while (compare != NULL) {
            if (current->value[0] > compare->value[0]) {
                tmp = current->value;
                current->value = compare->value;
                compare->value = tmp;
            }
            compare = compare->next;
        }
        current = current->next;
    }
}
