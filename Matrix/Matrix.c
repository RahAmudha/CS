/***
* Rahul Amudhasgaran
* ramudhas
* 2023 Fall CSE101 PA{4}
* Matrix.c
* Implementation of Matrix ADT
***/ 


#include <stdio.h>
#include "List.h"
#include "Matrix.h"
#include <stdlib.h>
#include <stdbool.h>

typedef struct EntryObj *Entry;
struct EntryObj {
    int column;
    double value;
} EntryObj;
typedef struct MatrixObj {
    List *matrix;
    int NNZ;
    int size;
} MatrixObj;
Matrix newMatrix (int n) {
    Matrix m = malloc (sizeof (MatrixObj));
    m->matrix = malloc ((n + 1) * sizeof (List));
    for (int i = 1; i <= n; ++i) {
        m->matrix [i] = newList ();
    } m->NNZ = 0;
    m->size = n; 
    return m;
}
Entry newEntry (int c, double val) {
    Entry e = malloc (sizeof (EntryObj));
    e->column = c;
    e->value = val;
    return e;
}
void freeMatrix (Matrix* pM) {
    for (int i = 1; i <= (*pM)->size; ++i) {
        List ithRow = (*pM)->matrix [i];
        if (length (ithRow) == 0) continue;
        for (moveFront (ithRow); index (ithRow) >= 0; moveNext (ithRow)) {
            free ((Entry) get (ithRow));
        } clear (ithRow);
    } (*pM)->NNZ = 0;
    for (int i = 1; i <= (*pM)->size; ++i) {
        freeList (&((*pM)->matrix [i]));
    } (*pM)->size = 0;
    free ((*pM)->matrix);
    free (*pM);
    pM = NULL;
}
int size (Matrix M) {
    return M->size;
}
int NNZ (Matrix M) {
    return M->NNZ;
}
int equals (Matrix A, Matrix B) {
    if (size (A) != size (B)) return 0;
    if (NNZ (A) != NNZ (B)) return 0;
    for (int i = 1; i <= A->size; ++i) {
        List ithRowA = A->matrix [i]; List ithRowB = B->matrix [i];
        if (length (ithRowA) != length (ithRowB)) return 0;
        moveFront (ithRowA); moveFront (ithRowB);
        while (index (ithRowA) >= 0) {
            if (((Entry) get (ithRowA))->column != ((Entry) get (ithRowB))->column || ((Entry) get (ithRowA))->value != ((Entry) get (ithRowB))->value) return 0;
            moveNext (ithRowA); moveNext (ithRowB);
        }
    } return 1;
}
void makeZero (Matrix M) {
    for (int i = 1; i <= M->size; ++i) {
        List ithRow = M->matrix [i];
        if (length (ithRow) == 0) continue;
        for (moveFront (ithRow); index (ithRow) >= 0; moveNext (ithRow)) {
            free (((Entry) get (ithRow)));
        } clear (ithRow);
    } M->NNZ = 0;
}
void changeEntry (Matrix M, int i, int j, double x) {
    if (i > M->size || j > M->size) return; //error checking
    List ithRow = M->matrix [i];
    if (length (ithRow)) {
        for (moveFront (ithRow); index (ithRow) >= 0; moveNext (ithRow)) {
            if (((Entry) get (ithRow))->column == j) {
                if (!x) {
                    free (((Entry) get (ithRow)));
                    delete (ithRow);
                    --(M->NNZ);
                    return;
                } ((Entry) get (ithRow))->value = x;
                return;
            }
            if (((Entry) get (ithRow))->column > j && x) {
                Entry e = newEntry (j, x);
                insertBefore (ithRow, e);
                ++(M->NNZ);
                return;
            }
        } 
    } if (x) {
        Entry e = newEntry (j, x);
        append (ithRow, e);
        ++(M->NNZ);
    }
}
Matrix copy (Matrix A) {
    Matrix newA = newMatrix (size (A));
    for (int i = 1; i <= newA->size; ++i) {
        List ithRow = A->matrix [i];
        for (moveFront (ithRow); index (ithRow) >= 0; moveNext (ithRow)) changeEntry (newA, i, ((Entry) get (ithRow))->column, ((Entry) get (ithRow))->value);
    } return newA;
}
Matrix transpose (Matrix A) {
    Matrix AT = newMatrix (size (A));
    for (int i = 1; i <= A->size; ++i) {
        List ithRow = A->matrix [i];
        for (moveFront (ithRow); index (ithRow) >= 0; moveNext (ithRow)) changeEntry (AT, ((Entry) get (ithRow))->column, i, ((Entry) get (ithRow))->value);
    } return AT;
}
Matrix scalarMult (double x, Matrix A) {
    Matrix scalarMatrix = copy (A);
    for (int i = 1; i <= scalarMatrix->size; ++i) {
        List ithRow = scalarMatrix->matrix [i];
        if (length (ithRow) == 0) continue;
        for (moveFront(ithRow); index(ithRow) >= 0; moveNext(ithRow)) ((Entry) get (ithRow))->value *= x;
    } return scalarMatrix;
}
Matrix sum (Matrix A, Matrix B) {
    if (equals (A, B)) return scalarMult (2, A);
    Matrix sum = copy (A);
    for (int i = 1; i <= size (sum); ++i) {
        List ithRowA = A->matrix [i]; List ithRowB = B->matrix [i];
        if (length (ithRowB) == 0) continue;
        for (moveFront (ithRowB); index (ithRowB) >= 0; moveNext (ithRowB)) {
            changeEntry (sum, i, ((Entry) get (ithRowB))->column, ((Entry) get (ithRowB))->value);
        } moveFront (ithRowA); moveFront (ithRowB);
        while (index (ithRowA) >= 0 && index (ithRowB) >= 0) {
            if (((Entry) get (ithRowA))->column == ((Entry) get (ithRowB))->column) {
                changeEntry (sum, i, ((Entry) get (ithRowA))->column, ((Entry) get (ithRowB))->value + ((Entry) get (ithRowA))->value);
                moveNext (ithRowA); moveNext (ithRowB);
                continue;
            } if (((Entry) get (ithRowA))->column < ((Entry) get (ithRowB))->column) moveNext (ithRowA);
            else if (((Entry) get (ithRowA))->column > ((Entry) get (ithRowB))->column) moveNext (ithRowB);
        }
    } return sum;
}
Matrix diff (Matrix A, Matrix B) {
    Matrix negB = scalarMult (-1, B);
    Matrix diff = sum (A, negB);
    freeMatrix (&negB);
    return diff;
}
double vectorDot (List A, List B) {
    double sum = 0;
    moveFront (A); moveFront (B);
    while (index (A) >= 0 && index (B) >= 0) {
        if (((Entry) get (A))->column == ((Entry) get (B))->column) {
            sum += (((Entry) get (A))->value * ((Entry) get (B))->value);
            moveNext (A); moveNext (B);
        } else if (((Entry) get (A))->column < ((Entry) get (B))->column) moveNext (A);
        else if (((Entry) get (A))->column > ((Entry) get (B))->column) moveNext (B);
    } return sum;
}
Matrix product (Matrix A, Matrix B) {
    Matrix BT = transpose (B);
    Matrix prod = newMatrix (size (A));
    for (int i = 1; i <= A->size; ++i) {
        for (int j = 1; j <= BT-> size; ++j) {
            List ithRow = A->matrix [i];
            List jthRow = BT->matrix [j];
            if (length (ithRow) && length (jthRow)) {
                double vdot = vectorDot (ithRow, jthRow);
                changeEntry (prod, i, j, vdot);
            }
        }
    } freeMatrix (&BT);
    return prod;
}
void printMatrix (FILE* out, Matrix M) {
    for (int i = 1; i <= M->size; ++i) {
        List ithRow = M->matrix [i];
        if (length (ithRow)) {
            fprintf (out, "%d: ", i);
            for(moveFront(ithRow); index(ithRow)>=0; moveNext(ithRow)){
                fprintf(out, "(%d, %0.1f) ", ((Entry) get (ithRow))->column, ((Entry) get (ithRow))->value);
            } fprintf (out, "\n");
        }
    }
}
