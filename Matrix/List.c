/***
* Rahul Amudhasgaran
* ramudhas
* 2023 Fall CSE101 PA{4}
* List.c
* Modded List ADT Program
***/ 

#include <stdio.h>
#include "List.h"
#include <stdbool.h>
#include <stdlib.h>

typedef struct NodeObj *Node;
struct NodeObj {
    void *point;
    Node next;
    Node back;
} NodeObj;
typedef struct ListObj {
    Node head;
    Node tail;
    Node cursor;
    int length;
    int index;
} ListObj;
List newList(void) {
    List L = calloc (1, sizeof (ListObj));
    L->cursor = NULL;
    L->head = NULL;
    L->tail = NULL;
    L->length = 0;
    L->index = -1;
    return L;
}
Node newNode (void *x) {
    Node node = calloc (1, sizeof (NodeObj));
    node->point = x;
    node->back = NULL;
    node->next = NULL;
    return node;
}
void freeList (List* pL) {
    clear (*pL);
    free (*pL);
    pL = NULL;
}
int length (List L) {
    return L->length;
}
int index (List L) {
    if (L->cursor != NULL) {
        return L->index;
    } return -1;
}
void* front (List L) {
    if (length (L)) {
        return L->head->point;
    } return NULL;
}
void* back (List L) {
    if (length (L)) {
        return L->tail->point;
    } return NULL;
}
void* get (List L) {
    if (length (L) && index (L) >= 0)
        return L->cursor->point;
    return NULL;
}
void clear (List L) {
    while (length (L)) {
        moveFront (L);
        deleteFront (L);
    }
}
void set (List L, void *x) {
    if (length (L) && L->index >= 0) {
        L->cursor->point = x;
    }
}
void moveFront (List L) {
    if (length (L)) {
        L->cursor = L->head;
        L->index = 0;
    }
}
void moveBack (List L) {
    if (length (L)) {
        L->cursor = L->tail;
        L->index = L->length - 1;
    }
}
void movePrev (List L) {
    if (L->index < 0) {
        exit(1);
    }
    if (L->index > 0) {
        L->cursor = L->cursor->back;
        --(L->index);
    } else {
        L->cursor = NULL;
        L->index = -1;
    }
}
void moveNext (List L) {
    if (L->index < 0) {
        exit(1);
    }
    if (L->index < L->length) {
        L->cursor = L->cursor->next;
        ++(L->index); 
    } else {
        L->cursor = NULL;
        L->index = -1;
    }
}
void prepend (List L, void *x) {
    Node node = newNode (x);
    if (length (L)) {
        node->next = L->head;
        L->head->back = node;
        L->head = node;
    } else {
        L->tail = node;
        L->head = node;
    }
    
    ++(L->length);
    if (L->cursor)
    ++(L->index);
}
void append (List L, void *x) {
    Node node = newNode (x);
    if (length (L)) {
        node->back = L->tail;
        L->tail->next = node;
        L->tail = node;
    } else {
        L->tail = node;
        L->head = node;
    }
    
    ++(L->length);
}
void insertBefore (List L, void *x) {
    if (!length (L) || index < 0) {
        exit (1);
    } 
    if (index (L) == 0) {
        Node node = newNode (x);
        L->head->back = node;
        node->next = L->head;
        L->head = node;
        ++(L->length);
        ++(L->index);
    } else {
        Node node = newNode (x);
        L->cursor->back->next = node; 
        node->back = L->cursor->back;
        L->cursor->back = node;
        node->next = L->cursor;
        ++(L->length);
        ++(L->index);
    }
}
void insertAfter (List L, void *x) {
    if (!length (L) || index < 0) {
        exit (1);
    }
    if (index (L) == length (L) - 1) {
        Node node = newNode (x);
        L->tail->next = node;
        node->back = L->tail;
        L->tail = node;
        ++(L->length);
    }
    else {
        Node node = newNode (x);
        L->cursor->next->back = node;
        node->next = L->cursor->next;
        L->cursor->next = node;
        node->back = L->cursor;
        ++(L->length);
    }
} 
void deleteFront (List L) {
    if (index (L) == 0) {
        L->cursor = NULL;
        L->index = -1;
    }
    if (length (L) > 1) { 
        L->head = L->head->next;
        L->head->back->next = NULL;
        free (L->head->back);
        L->head->back = NULL;
    } else if (length (L) == 1) {
        free (L->head);
    } --(L->length);
    if (L->cursor)
        --(L->index);
}
void deleteBack (List L) {
    if (index (L) == length (L) - 1) {
        L->cursor = NULL;
        L->index = -1;
    }
    if (length (L) > 1) {
        L->tail = L->tail->back;
        L->tail->next->back = NULL;
        free (L->tail->next);
        L->tail->next = NULL;
    } else if (length (L) == 1) {
        free (L->tail);
    } --(L->length);
}
void delete (List L) {
    if (length (L) && index (L) >= 0) {
        if (index (L) == length (L) - 1) {
            deleteBack (L); return;
        }
        if (index (L) == 0) {
            deleteFront (L); return;
        }
        if (L->cursor->next && L->cursor->back) {
            L->cursor->next->back = L->cursor->back;
            L->cursor->back->next = L->cursor->next;
            free (L->cursor);
            L->cursor = NULL;
            L->index = -1;
            --(L->length);
        }
    }
}
void printList (FILE* out, List L) {
    Node tempNode = L->head;
    while (tempNode) {
        fprintf (out, "%p ", tempNode->point);
        tempNode = tempNode->next;
    }
}