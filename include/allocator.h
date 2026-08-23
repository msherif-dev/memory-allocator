#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stddef.h>

/*
==========================================
              Public API
==========================================
*/

void *my_malloc(size_t size);

void my_free(void *ptr);

void heap_dump(void);

void print_heap_status(void);

#endif /* ALLOCATOR_H */
