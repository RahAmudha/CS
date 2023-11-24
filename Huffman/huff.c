#include "bitwriter.h"
#include "node.h"
#include "pq.h"

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define OPTIONS "i:o:h"

typedef struct Code {
    uint64_t code;
    uint8_t code_length;
} Code;

uint64_t fill_histogram(Buffer *inbuf, double *histogram);
Node *create_tree(double *histogram, uint16_t *num_leaves);
void fill_code_table(Code *code_table, Node *node, uint64_t code, uint8_t code_length);
void huff_compress_file(BitWriter *outbuf, Buffer *inbuf, uint32_t filesize, uint16_t num_leaves,
    Node *code_tree, Code *code_table);
void huff_write_tree(BitWriter *outbuf, Node *node);
void free_tree(Node **code_tree);
void print_help(void);

int main(int argc, char **argv) {
    char *input_name = "";
    char *output_name = "";
    char ch;
    Buffer *inbuf;
    BitWriter *outbuf;

    while ((ch = getopt(argc, argv, OPTIONS)) != -1) {
        switch (ch) {
        case 'i': input_name = optarg; break;
        case 'o': output_name = optarg; break;
        case 'h': print_help(); return 0;
        }
    }
    if (strcmp(input_name, "") == 0 || strcmp(output_name, "") == 0) {
        fprintf(stderr, "huff:   no input or output file given.");
        exit(0);
    }

    inbuf = read_open(input_name);
    double *hist = malloc(256 * sizeof(double));
    uint64_t filesize = fill_histogram(inbuf, hist);
    read_close(&inbuf);

    uint16_t num_leaves;
    Node *code_tree = create_tree(hist, &num_leaves);
    free(hist);

    Code *code_table = (Code *) calloc(256, sizeof(Code));
    fill_code_table(code_table, code_tree, 0, 0);

    inbuf = read_open(input_name);
    outbuf = bit_write_open(output_name);
    huff_compress_file(outbuf, inbuf, filesize, num_leaves, code_tree, code_table);
    read_close(&inbuf);
    bit_write_close(&outbuf);
    free_tree(&code_tree);
    free(code_table);

    return 0;
}

uint64_t fill_histogram(Buffer *inbuf, double *histogram) {
    for (uint16_t i = 0; i < 256; ++i) {
        histogram[i] = 0;
    }
    uint8_t byte;
    uint64_t filesize = 0;
    while (read_uint8(inbuf, &byte)) {
        ++histogram[byte];
        ++filesize;
    }
    ++histogram[0x00];
    ++histogram[0xff];
    return filesize;
}

Node *create_tree(double *histogram, uint16_t *num_leaves) {
    *num_leaves = 0;
    PriorityQueue *pq = pq_create();
    for (uint16_t i = 0; i < 256; ++i) {
        if (histogram[i]) {
            Node *n = node_create(i, histogram[i]);
            ++(*num_leaves);
            enqueue(pq, n);
        }
    }
    Node *left;
    Node *right;
    while (!pq_size_is_1(pq)) {
        dequeue(pq, &left);
        dequeue(pq, &right);
        Node *p = node_create(0, left->weight + right->weight);
        p->left = left;
        p->right = right;
        enqueue(pq, p);
    }
    dequeue(pq, &left);
    pq_free(&pq);
    return left;
}

void fill_code_table(Code *code_table, Node *node, uint64_t code, uint8_t code_length) {
    if (node->left) {
        fill_code_table(code_table, node->left, code, code_length + 1);
        code |= 1 << code_length;
        fill_code_table(code_table, node->right, code, code_length + 1);
    } else {
        code_table[node->symbol].code = code;
        code_table[node->symbol].code_length = code_length;
    }
}

void huff_compress_file(BitWriter *outbuf, Buffer *inbuf, uint32_t filesize, uint16_t num_leaves,
    Node *code_tree, Code *code_table) {
    bit_write_uint8(outbuf, 'H');
    bit_write_uint8(outbuf, 'C');
    bit_write_uint32(outbuf, filesize);
    bit_write_uint16(outbuf, num_leaves);
    huff_write_tree(outbuf, code_tree);

    uint8_t byte;
    uint64_t code;
    uint8_t code_length;
    while (read_uint8(inbuf, &byte)) {
        code = code_table[byte].code;
        code_length = code_table[byte].code_length;
        for (uint8_t i = 0; i < code_length; ++i) {
            bit_write_bit(outbuf, code & 1);
            code >>= 1;
        }
    }
}

void huff_write_tree(BitWriter *outbuf, Node *node) {
    if (node->left) {
        huff_write_tree(outbuf, node->left);
        huff_write_tree(outbuf, node->right);
        bit_write_bit(outbuf, 0);
    } else {
        bit_write_bit(outbuf, 1);
        bit_write_uint8(outbuf, node->symbol);
    }
}

void free_tree(Node **code_tree) {
    if ((*code_tree)->left) {
        free_tree(&((*code_tree)->left));
        free_tree(&((*code_tree)->right));
    }
    node_free(code_tree);
}

void print_help(void) {
    printf(
        "Usage: huff -i infile -o outfile\n       huff -v -i infile -o outfile\n       huff -h\n");
}
