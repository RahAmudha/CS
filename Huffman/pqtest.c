/*
* File:     pqtest.c
* Purpose:  Test pq.c
* Author:   Kerry Veenstra
*/

#include "pq.h"

#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

/*
* Used when comparing floating-point values for equality.
*/
#define EPSILON 1e-10

/*
* This macro removes a node/tree from the queue and verifies its weight.
* The macro must be used in a scope where a bool variable called verbose
* exists.  If verbose is true, then the dequeued weight is printed.
*/
#define D(N, W)                                                                                    \
    assert(dequeue(q, &(N)));                                                                      \
    if (verbose)                                                                                   \
        printf("dequeue %f\n", (N)->weight);                                                       \
    assert(fabs((N)->weight - (W)) < EPSILON)

int main(int argc, char **argv) {
    /*
    * Poor man's argument checking: is "-v" the first command-line argument?
    * Ignore all other arguments.
    */
    bool verbose = argc > 1 && strcmp(argv[1], "-v") == 0;

    if (!verbose)
        printf("Use \"pqtest -v\" to print trace information.\n");

    Node *n;

    /*
    * Create a priority queue and eight nodes.
    */
    PriorityQueue *q = pq_create();

    Node *n1 = node_create('1', 5.0);
    Node *n2 = node_create('2', 15.0);
    Node *n3 = node_create('3', 10.0);
    Node *n4 = node_create('4', 60.0);
    Node *n5 = node_create('5', 45.0);
    Node *n6 = node_create('6', 12.0);
    Node *n7 = node_create('7', 40.0);
    Node *n8 = node_create('8', 28.0);

    /*
    * Enqueue the nodes and then dequeue them, checking that their order in
    * the priority queue is correct.  On the way, check the return values
    * of pq_size_is_1() and pq_is_empty().
    */
    assert(!pq_size_is_1(q));
    assert(pq_is_empty(q));

    enqueue(q, n1);
    assert(pq_size_is_1(q));
    assert(!pq_is_empty(q));

    enqueue(q, n2);
    assert(!pq_size_is_1(q));
    assert(!pq_is_empty(q));

    enqueue(q, n3);
    enqueue(q, n4);
    enqueue(q, n5);
    enqueue(q, n6);
    enqueue(q, n7);
    enqueue(q, n8);

    if (verbose)
        pq_print(q);

    D(n, 5.0);
    D(n, 10.0);
    D(n, 12.0);
    D(n, 15.0);
    D(n, 28.0);
    D(n, 40.0);
    assert(!pq_size_is_1(q));
    assert(!pq_is_empty(q));

    D(n, 45.0);
    assert(pq_size_is_1(q));
    assert(!pq_is_empty(q));

    D(n, 60.0);
    assert(!pq_size_is_1(q));
    assert(pq_is_empty(q));

    assert(!dequeue(q, &n));

    /*
    * Make a couple trees from the nodes and enqueue them.
    *
    *         n4
    *        /  \
    *      n2    n5
    *     /  \
    *   n1    n3
    */
    n2->left = n1;
    n2->right = n3;
    n4->left = n2;
    n4->right = n5;

    /*
    *      n7
    *     /  \
    *   n6    n8
    */
    n7->left = n6;
    n7->right = n8;

    assert(!pq_size_is_1(q));
    assert(pq_is_empty(q));

    enqueue(q, n7);
    assert(pq_size_is_1(q));
    assert(!pq_is_empty(q));

    enqueue(q, n4);
    assert(!pq_size_is_1(q));
    assert(!pq_is_empty(q));

    /*
    * Dequeue the trees, and merge them into one tree, which is enqueued.
    */
    Node *left;
    Node *right;

    if (verbose)
        pq_print(q);

    D(left, 40.0);
    assert(pq_size_is_1(q));
    assert(!pq_is_empty(q));

    D(right, 60.0);
    assert(!pq_size_is_1(q));
    assert(pq_is_empty(q));

    Node *new_node = node_create('$', 100.0);
    new_node->left = left;
    new_node->right = right;

    enqueue(q, new_node);
    assert(pq_size_is_1(q));
    assert(!pq_is_empty(q));

    if (verbose)
        pq_print(q);

    D(n, 100.0);
    assert(pq_is_empty(q));
    assert(!pq_size_is_1(q));

    assert(!dequeue(q, &n));

    /*
    * Free everything.
    */
    node_free(&n1);
    node_free(&n2);
    node_free(&n3);
    node_free(&n4);
    node_free(&n5);
    node_free(&n6);
    node_free(&n7);
    node_free(&n8);
    node_free(&new_node);

    assert(n1 == NULL);
    assert(n2 == NULL);
    assert(n3 == NULL);
    assert(n4 == NULL);
    assert(n5 == NULL);
    assert(n6 == NULL);
    assert(n7 == NULL);
    assert(n8 == NULL);
    assert(new_node == NULL);

    pq_free(&q);
    assert(q == NULL);

    printf("pqtest, as it is, reports no errors\n");
    return 0;
}
