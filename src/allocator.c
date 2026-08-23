#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stddef.h>
#include <unistd.h>
#include <stdint.h>

#include "allocator.h"

/*
==========================================
                Alignment
==========================================
*/

#define ALIGNMENT 8


/*
==========================================
             Block Metadata
==========================================
*/

typedef struct block
{
    size_t size;

    int free;

} block_t;


/*
==========================================
              Heap State
==========================================
*/

static void *heap_start = NULL;

static void *heap_end = NULL;


/*
==========================================
              Alignment
==========================================
*/

static size_t align_size(size_t size)
{
    return (size + ALIGNMENT - 1)
           & ~(ALIGNMENT - 1);
}


/*
==========================================
            Metadata Size
==========================================
*/

static size_t metadata_size(void)
{
    return align_size(sizeof(block_t));
}


/*
==========================================
          Request Memory From OS
==========================================
*/

static void *request_memory(size_t size)
{
    void *block = sbrk((intptr_t)size);

    if (block == (void *)-1)
    {
        return NULL;
    }

    return block;
}


/*
==========================================
            Get Next Block
==========================================
*/

static block_t *get_next_block(block_t *block)
{
    return (block_t *)(
        (unsigned char *)block
        + metadata_size()
        + block->size
    );
}


/*
==========================================
          Check Block Validity
==========================================
*/

static int is_valid_block(block_t *block)
{
    unsigned char *start =
        (unsigned char *)block;

    unsigned char *end =
        start
        + metadata_size()
        + block->size;


    if (heap_start == NULL ||
        heap_end == NULL)
    {
        return 0;
    }


    if (start < (unsigned char *)heap_start)
    {
        return 0;
    }


    if (end > (unsigned char *)heap_end)
    {
        return 0;
    }


    return 1;
}


/*
==========================================
          Find Free Block
          First Fit
==========================================
*/

static block_t *find_free_block(size_t size)
{
    if (heap_start == NULL)
    {
        return NULL;
    }


    block_t *current =
        (block_t *)heap_start;


    while ((unsigned char *)current
           < (unsigned char *)heap_end)
    {
        if (!is_valid_block(current))
        {
            return NULL;
        }


        if (current->free == 1 &&
            current->size >= size)
        {
            return current;
        }


        current =
            get_next_block(current);
    }


    return NULL;
}


/*
==========================================
                Split Block
==========================================
*/

static void split_block(block_t *block,
                        size_t size)
{
    if (block->size <= size)
    {
        return;
    }


    size_t remaining =
        block->size - size;


    /*
        نحتاج مساحة للـ Metadata
        والـ Data الجديدة
    */

    if (remaining <=
        metadata_size() + ALIGNMENT)
    {
        return;
    }


    block_t *new_block =
        (block_t *)(
            (unsigned char *)block
            + metadata_size()
            + size
        );


    new_block->size =
        remaining - metadata_size();


    new_block->size =
        align_size(new_block->size);


    new_block->free = 1;


    block->size = size;
}


/*
==========================================
              Coalescing
==========================================
*/

static void coalesce_blocks(void)
{
    if (heap_start == NULL)
    {
        return;
    }


    block_t *current =
        (block_t *)heap_start;


    while ((unsigned char *)current
           < (unsigned char *)heap_end)
    {
        if (!is_valid_block(current))
        {
            return;
        }


        block_t *next =
            get_next_block(current);


        if ((unsigned char *)next
            >= (unsigned char *)heap_end)
        {
            break;
        }


        if (!is_valid_block(next))
        {
            return;
        }


        if (current->free == 1 &&
            next->free == 1)
        {

            current->size +=
                metadata_size()
                + next->size;

            continue;
        }


        current = next;
    }
}


/*
==========================================
                my_malloc
==========================================
*/

void *my_malloc(size_t size)
{
    if (size == 0)
    {
        return NULL;
    }


    /*
        Alignment
    */

    size = align_size(size);


    /*
        هل يوجد Block حرة؟
    */

    block_t *block =
        find_free_block(size);


    /*
        نعم
    */

    if (block != NULL)
    {
        split_block(block, size);

        block->free = 0;

        return (void *)(block + 1);
    }


    /*
        لا توجد Block مناسبة.

        نحتاج مساحة جديدة من OS عن طريق sbrk.
    */

    size_t total_size =
        metadata_size() + size;


    block =
        (block_t *)request_memory(total_size);


    if (block == NULL)
    {
        return NULL;
    }


    /*
        أول Allocation
    */

    if (heap_start == NULL)
    {
        heap_start = block;
    }


    /*
        Metadata
    */

    block->size = size;

    block->free = 0;


    /*
        تحديث نهاية الـ Heap
    */

    heap_end =
        (unsigned char *)block
        + total_size;


    /*
        إرجاع User Data
    */

    return (void *)(block + 1);
}


/*
==========================================
                 my_free
==========================================
*/

void my_free(void *ptr)
{

    if (ptr == NULL)
    {
        return;
    }


    block_t *block =
        (block_t *)((unsigned char *)ptr
                     - metadata_size());


    if (!is_valid_block(block))
    {
        printf(
            "ERROR: invalid pointer passed to free\n"
        );

        return;
    }


    /*
        Double-Free Check
    */

    if (block->free == 1)
    {
        printf(
            "ERROR: double free detected\n"
        );

        return;
    }


    block->free = 1;


    coalesce_blocks();
}

/*
==========================================
              Heap Dump
==========================================
*/

void heap_dump(void)
{
    printf("\n");
    printf("========== HEAP DUMP ==========\n");


    if (heap_start == NULL)
    {
        printf("Heap is empty.\n");

        printf("===============================\n");

        return;
    }


    block_t *current =
        (block_t *)heap_start;


    int index = 0;


    while ((unsigned char *)current
           < (unsigned char *)heap_end)
    {
        if (!is_valid_block(current))
        {
            printf(
                "ERROR: corrupted block\n"
            );

            break;
        }


        printf("\n");

        printf("Block #%d\n", index);

        printf("Address : %p\n",
               (void *)current);

        printf("Size    : %zu\n",
               current->size);

        printf("Status  : %s\n",
               current->free
                   ? "FREE"
                   : "USED");


        current =
            get_next_block(current);

        index++;
    }


    printf("\n");
    printf("===============================\n");
}


/*
==========================================
              Heap Status
==========================================
*/

void print_heap_status(void)
{
    printf("\n");

    printf("Heap Start : %p\n",
           heap_start);

    printf("Heap End   : %p\n",
           heap_end);


    if (heap_start != NULL &&
        heap_end != NULL)
    {
        printf(
            "Heap Size  : %zu Bytes\n",
            (size_t)(
                (unsigned char *)heap_end
                - (unsigned char *)heap_start
            )
        );
    }
    else
    {
        printf("Heap Size  : 0 Bytes\n");
    }
}
