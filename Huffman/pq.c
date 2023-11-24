#include "pq.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct ListElement {
    Node *tree;
    struct ListElement *next;
} ListElement;

struct PriorityQueue {
    ListElement *list;
};

bool pq_less_than(Node *n1, Node *n2);

PriorityQueue *pq_create(void) {
    PriorityQueue *pq = (PriorityQueue *) calloc(1, sizeof(PriorityQueue));
    return pq;
}

void pq_free(PriorityQueue **q) {
    free(*q);
    *q = NULL;
}

bool pq_is_empty(PriorityQueue *q) {
    return q->list == NULL;
}

bool pq_size_is_1(PriorityQueue *q) {
    if (!pq_is_empty(q) && q->list->next == NULL) {
        return true;
    }
    return false;
}

void enqueue(PriorityQueue *q, Node *tree) {
    ListElement *e = (ListElement *) calloc(1, sizeof(ListElement));
    e->tree = tree;
    if (pq_is_empty(q)) {
        q->list = e;
    } else if (pq_less_than(e->tree, q->list->tree)) {
        e->next = q->list;
        q->list = e;
    } else {
        ListElement *i;
        for (i = q->list; i->next != NULL; i = i->next) {
            if (pq_less_than(e->tree, i->next->tree)) {
                e->next = i->next;
                i->next = e;
                return;
            }
        }
        i->next = e;
    }
}

bool dequeue(PriorityQueue *q, Node **tree) {
    if (pq_is_empty(q))
        return false;
    ListElement *e;
    e = q->list;
    q->list = q->list->next;
    *tree = e->tree;
    free(e);
    return true;
}

void pq_print(PriorityQueue *q) {
    assert(q != NULL);
    ListElement *e = q->list;
    int position = 1;
    while (e != NULL) {
        if (position++ == 1) {
            printf("=============================================\n");
        } else {
            printf("---------------------------------------------\n");
        }
        node_print_tree(e->tree, '<', 2);
        e = e->next;
    }
    printf("=============================================\n");
}

bool pq_less_than(Node *n1, Node *n2) {
    if (n1->weight < n2->weight)
        return true;
    if (n1->weight > n2->weight)
        return false;
    return n1->symbol < n2->symbol;
}
