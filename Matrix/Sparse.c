/***
* Rahul Amudhasgaran
* ramudhas
* 2023 Fall CSE101 PA{4}
* Sparse.c
* Main File, runs Matrix functions with sparse Matrix ADT
***/ 

#include "Matrix.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

int main(int argc, char * argv[]) {
    FILE *in = NULL;
    FILE *out = NULL;
    // check command line for correct number of arguments
    if (argc != 3) {
       printf ("Usage: %s <input file> <output file>\n", argv [0]);
       exit (1);
    } // open files for reading and writing 
    in = fopen (argv [1], "r");
    if (in == NULL) {
       fprintf(stderr, "Unable to open file %s for reading\n", argv [1]);
       exit (1);
    } out = fopen (argv [2], "w");
    if (out == NULL) {
       fprintf (stderr, "Unable to open file %s for writing\n", argv [2]);
       exit (1);
    } 
    int size;
    int NNZa;
    int NNZb;
    Matrix A;
    Matrix B;
    if (fscanf (in, "%d %d %d", &size, &NNZa, &NNZb) == 3) {
        A = newMatrix (size);
        B = newMatrix (size);
    } 
    int i;
    int j;
    double val;
    int iter = 0;
    while (fscanf (in, "%d %d %lf", &i, &j, &val) == 3) {
        if (iter < NNZa) changeEntry (A, i, j, val);
        else changeEntry (B, i, j, val);
        ++iter;
    }

    Matrix scale = scalarMult (1.5, A);
    Matrix sumOfAB = sum (A, B);
    Matrix sumOfAA = sum (A, A);
    Matrix diffOfBA = diff (B, A);
    Matrix diffOfAA = diff (A, A);
    Matrix AT = transpose (A);
    Matrix prodOfAB = product (A, B);
    Matrix prodOfBB = product (B, B);


    fprintf (out, "A has %d non-zero entries:\n", NNZa);
    printMatrix (out, A);
    fprintf (out, "\n");

    fprintf (out, "B has %d non-zero entries:\n", NNZb);
    printMatrix (out, B);
    fprintf (out, "\n");

    fprintf (out, "(1.5)*A =\n");
    printMatrix (out, scale);
    fprintf (out, "\n");

    fprintf (out, "A+B =\n");
    printMatrix (out, sumOfAB);
    fprintf (out, "\n");

    fprintf (out, "A+A =\n");
    printMatrix (out, sumOfAA);
    fprintf (out, "\n");

    fprintf (out, "B-A =\n");
    printMatrix (out, diffOfBA);
    fprintf (out, "\n");

    fprintf (out, "A-A =\n");
    printMatrix (out, diffOfAA);
    fprintf (out, "\n");

    fprintf (out, "Transpose (A) =\n");
    printMatrix (out, AT);
    fprintf (out, "\n");

    fprintf (out, "A*B =\n");
    printMatrix (out, prodOfAB);
    fprintf (out, "\n");

    fprintf (out, "B*B =\n");
    printMatrix (out, prodOfBB);
    fprintf (out, "\n");

    freeMatrix (&A);
    freeMatrix (&B);
    freeMatrix (&scale);
    freeMatrix (&sumOfAB);
    freeMatrix (&sumOfAA);
    freeMatrix (&diffOfBA);
    freeMatrix (&diffOfAA);
    freeMatrix (&AT);
    freeMatrix (&prodOfAB);
    freeMatrix (&prodOfBB);

    fclose (in);
    fclose (out);
}