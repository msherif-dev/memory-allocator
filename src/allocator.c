#include <stdio.h>
#include <stddef.h>

#define HEAP_SIZE 1024
# define ALIGNMENT 8


static unsigned char heap[HEAP_SIZE];

static size_t heap_used = 0;


typedef struct block
{
    size_t size;

    int free;

} block_t;


static size_t align_size(size_t size){

    return(size + ALIGNMENT - 1) 
            & ~ (ALIGNMENT - 1); 

}

static size_t metadata_size(void){
    return align_size (sizeof(block_t));
}


static block_t *find_free_block(size_t size)
{
    block_t *current = (block_t *)heap;

    while ((unsigned char *)current < heap + heap_used)
    {
        if (current->free == 1 &&
            current->size >= size)
        {
            return current;
        }

        current = (block_t *)(
            (unsigned char *)current +
            metadata_size() +
            current->size
        );
    }

    return NULL;
}


static void split_block(block_t *block,
                        size_t size)
{
    if (block->size <= size)
    {
        return;
    }

    size_t remaining =
        block->size - size;


    if (remaining <=
        metadata_size() + ALIGNMENT)
    {
        return;
    }

    block_t *new_block = (block_t *)((unsigned char *)block + metadata_size() + size);

    new_block->size =
        remaining - metadata_size();


    new_block->size =
        align_size(new_block->size);


    new_block->free = 1;


    block->size = size;
}


static void coalesce_blocks(void)
{
    block_t *current = (block_t *)heap;

    while ((unsigned char *)current < heap + heap_used)
    {
        block_t *next =
            (block_t *)(
                (unsigned char *)current +
                metadata_size()+
                current->size
            );


        if ((unsigned char *)next >= heap + heap_used)
        {
            break;
        }


        if (current->free == 1 &&
            next->free == 1)
        {

            current->size +=
                metadata_size()+
                next->size;


            continue;
        }

        current = next;
    }
}



void *my_malloc(size_t size)
{


    if (size == 0)
    {
        return NULL;
    }

    size = align_size(size);


    block_t *block =
        find_free_block(size);




    if (block != NULL)
    {
        split_block(block, size);

        block->free = 0;

        return (void *)(block + 1);
    }


    size_t total_size =
        metadata_size() + size;


    if (heap_used + total_size > HEAP_SIZE)
    {
        return NULL;
    }

    block =
        (block_t *)(heap + heap_used);


    block->size = size;

    block->free = 0;



    heap_used += total_size;


    return (void *)(block + 1);
}


void my_free(void *ptr)
{


    if (ptr == NULL)
    {
        return;
    }


    block_t *block = 
        (block_t *)((unsigned char *)ptr
         - metadata_size());

         
    block->free = 1;

    coalesce_blocks();
}


void print_heap_status(void)
{
    printf("Heap Size      : %d Bytes\n",
           HEAP_SIZE);

    printf("Heap Used      : %zu Bytes\n",
           heap_used);

    printf("Heap Remaining : %zu Bytes\n",
           HEAP_SIZE - heap_used);
}