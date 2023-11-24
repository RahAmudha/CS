/***
* Rahul Amudhasgaran
* ramudhas
* 2023 Fall CSE101 PA{4}
* ListTest.c
* Testing Client for List ADT
***/ 

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include"List.h"

int main(int argc, char* argv[]){
   
   List A = newList();
   List B = newList();
   List C = newList();
   int X[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
   int i, u=-1, v=-2, w=-3; 
   bool equal = false;

   // initialize Lists A and B
   for(i=1; i<=20; i++){
      append(A, &X[i]);
      prepend(B, &X[i]);
   }

   // print both lists of integers in forward direction
   for(moveFront(A); index(A)>=0; moveNext(A)){
      printf("%d ", *(int*)get(A));
   }
   printf("\n");
   for(moveFront(B); index(B)>=0; moveNext(B)){
      printf("%d ", *(int*)get(B));
   }
   printf("\n");

   // print both lists of integers in backward direction
   for(moveBack(A); index(A)>=0; movePrev(A)){
      printf("%d ", *(int*)get(A));
   }
   printf("\n");
   for(moveBack(B); index(B)>=0; movePrev(B)){
      printf("%d ", *(int*)get(B));
   }
   printf("\n");

   // make a shallow copy of A
   moveFront(A);
   while( index(A)>=0 ){
      append(C, get(A));
      moveNext(A);
   }
   // print the copy in forward direction
   for(moveFront(C); index(C)>=0; moveNext(C)){
      printf("%d ", *(int*)get(C));
   }
   printf("\n");

   // check shallow equality of A and C by comparing pointers
   equal = (length(A)==length(C));
   moveFront(A);
   moveFront(C);
   while( index(A)>=0 && equal){
      equal = ( get(A)==get(C) );
      moveNext(A);
      moveNext(C);
   }
   printf("A equals C is %s\n", (equal?"true":"false") );

   moveFront(A);
   for(i=0; i<5; i++) moveNext(A);     // at index 5
   printf("index(A)=%d\n", index(A));
   insertBefore(A, &u);                // now at index 6
   printf("index(A)=%d\n", index(A));
   for(i=0; i<9; i++) moveNext(A);     // at index 15
   printf("index(A)=%d\n", index(A));
   insertAfter(A, &v);                 // doesn't change index
   printf("index(A)=%d\n", index(A));
   for(i=0; i<5; i++) movePrev(A);     // at index 10
   printf("index(A)=%d\n", index(A));
   delete(A);                          // index is now undefined
   printf("index(A)=%d\n", index(A));
   moveBack(A);                        // now at index 20
   printf("index(A)=%d\n", index(A));
   for(i=0; i<10; i++) movePrev(A);    // at index 10
   printf("index(A)=%d\n", index(A));
   set(A, &w);

   // print A in forward and backward direction
   for(moveFront(A); index(A)>=0; moveNext(A)){
      printf("%d ", *(int*)get(A));
   }
   printf("\n");
   for(moveBack(A); index(A)>=0; movePrev(A)){
      printf("%d ", *(int*)get(A));
   }
   printf("\n");

   // check length of A, before and after clear()
   printf("%d\n", length(A));
   clear(A);
   printf("%d\n", length(A));

   freeList(&A);
   freeList(&B);
   freeList(&C);

   return(0);
}

/*
Output of this program:
1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1
20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1
1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
A equals C is true
index(A)=5
index(A)=6
index(A)=15
index(A)=15
index(A)=10
index(A)=-1
index(A)=20
index(A)=10
1 2 3 4 5 -1 6 7 8 9 -3 12 13 14 15 -2 16 17 18 19 20
20 19 18 17 16 -2 15 14 13 12 -3 9 8 7 6 -1 5 4 3 2 1
21
0
*/