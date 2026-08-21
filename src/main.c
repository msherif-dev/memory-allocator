#include <stdio.h>
#include <stddef.h>

void *my_malloc(size_t size);

void my_free(void *ptr);

void print_heap_status(void);

int main()
{
    print_heap_status();

    printf("\n");

    int *a = my_malloc(sizeof(int));

    double *b = my_malloc(sizeof(double));

    char *text = my_malloc(32);

    *a = 10;

    *b = 3.14;

    printf("%d\n", *a);

    printf("%.2f\n", *b);

    my_free(b);

    printf("\n");

    double *c = my_malloc(sizeof(double));

    printf("Old Address : %p\n", (void *)b);

    printf("New Address : %p\n", (void *)c);

    printf("\n");

    print_heap_status();

    return 0;
}