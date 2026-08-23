#include <stdio.h>
#include <stddef.h>

void *my_malloc(size_t size);

void my_free(void *ptr);

void print_heap_status(void);


int main(void)
{
    int *a = my_malloc(20);

    int *b = my_malloc(30);

    int *c = my_malloc(40);


    printf("A = %p\n", (void *)a);
    printf("B = %p\n", (void *)b);
    printf("C = %p\n", (void *)c);


    printf("\nFree A\n");

    my_free(a);


    printf("\nFree B\n");

    my_free(b);


    printf("\nAllocate 40 Bytes\n");

    int *d = my_malloc(40);

    printf("D = %p\n", (void *)d);


    printf("\nHeap Status\n");

    print_heap_status();


    return 0;
}