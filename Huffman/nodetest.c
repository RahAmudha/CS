/*
* File:     nodetest.c
* Purpose:  Test node.c
* Author:   Kerry Veenstra
*/

#include "node.h"

#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
* This macro makes a node and checks its data fields.
*/
#define N(N, S, W)                                                                                 \
    Node *N = node_create(S, W);                                                                   \
    assert(N->symbol == S);                                                                        \
    assert(N->weight == W);                                                                        \
    assert(N->left == NULL);                                                                       \
    assert(N->right == NULL);                                                                      \
    assert(N->code == 0);                                                                          \
    assert(N->code_length == 0);

int main(int argc, char **argv) {
    /*
    * Poor man's argument checking: is "-v" the first command-line argument?
    * Ignore all other arguments.
    */
    bool verbose = argc > 1 && strcmp(argv[1], "-v") == 0;

    if (!verbose)
        printf("Use \"nodetest -v\" to print trace information.\n");

    /*
    * Create and initialize some nodes.
    */
    N(n1, '1', 0.1);
    N(n2, '2', 0.4);
    N(n3, '3', 0.3);
    N(n4, '4', 1.0);
    N(n5, '5', 0.6);

    /*
    * Make a tree.
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

    if (verbose)
        node_print_tree(n4, '<', 0);

    /*
    * Free everything.
    */
    node_free(&n1);
    node_free(&n2);
    node_free(&n3);
    node_free(&n4);
    node_free(&n5);

    assert(n1 == NULL);
    assert(n2 == NULL);
    assert(n3 == NULL);
    assert(n4 == NULL);
    assert(n5 == NULL);

    printf("nodetest, as it is, reports no errors\n");
    return 0;
}
