#include <stdio.h>
#include <stddef.h>

#include "allocator.h"


int main(void)
{
    printf("===== INITIAL =====\n");

    print_heap_status();

    heap_dump();


    /*
    ======================================
             Allocation A
    ======================================
    */

    int *a = my_malloc(sizeof(int));

    *a = 100;


    printf("\n===== AFTER A =====\n");

    printf("A address: %p\n",
           (void *)a);

    printf("A value: %d\n",
           *a);

    print_heap_status();

    heap_dump();


    /*
    ======================================
             Allocation B
    ======================================
    */

    double *b = my_malloc(sizeof(double));

    *b = 3.14159;


    printf("\n===== AFTER B =====\n");

    printf("B address: %p\n",
           (void *)b);

    printf("B value: %f\n",
           *b);

    print_heap_status();

    heap_dump();


    /*
    ======================================
                 Free A
    ======================================
    */

    my_free(a);


    printf("\n===== AFTER FREE A =====\n");

    print_heap_status();

    heap_dump();


    /*
    ======================================
             Allocation C
             (should reuse A's freed space)
    ======================================
    */

    char *c = my_malloc(20);


    printf("\n===== AFTER C =====\n");

    printf("C address: %p\n",
           (void *)c);

    printf("Was A's space reused? %s\n",
           ((void *)c == (void *)a) ? "YES" : "NO");

    print_heap_status();

    heap_dump();


    /*
    ======================================
             Double-Free Test
    ======================================
    */

    printf("\n===== DOUBLE FREE TEST =====\n");

    my_free(b);

    printf("First free of B: should succeed silently\n");

    my_free(b);

    printf("Second free of B: should print an error above\n");


    /*
    ======================================
             Free Everything
    ======================================
    */

    my_free(c);


    printf("\n===== FINAL =====\n");

    print_heap_status();

    heap_dump();


    return 0;
}
