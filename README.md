<<<<<<< HEAD
# Custom C Memory Allocator (First-Fit & Coalescing)

A lightweight, custom dynamic memory allocator written in C. It implements core memory management concepts similar to `malloc` and `free` from the C standard library (`stdlib.h`) using a fixed-size heap buffer.

## Features

- **First-Fit Search**: Scans the heap sequentially to find the first block that accommodates the requested size.
- **Block Splitting**: Reduces internal fragmentation by dividing oversized free blocks into smaller allocations.
- **Adjacent Block Coalescing**: Merges neighboring free blocks during deallocation to prevent external fragmentation.
- **Pointer Arithmetic Safety**: Accurately computes block headers and metadata bounds.

## Memory Architecture

The allocator manages a `1024-byte` static array (`heap`) with the following block structure:
+-------------------+--------------------+
|  block_t Header   |    Payload Data    |
| (size, free flag) | (Allocated/Free)   |
+-------------------+--------------------+

- Each block starts with a header (`sizeof(block_t)`) containing metadata (`size` and `free` status).
- User pointers returned by `my_malloc` point directly to the **payload** area (just past the header).

## Getting Started

### Prerequisites

- Any standard C compiler (e.g., `gcc`, `clang`, or MSVC).

### Compilation & Execution

To compile and run the project using GCC:

```bash
# Compile the source code
gcc -Wall -Wextra -std=c99 main.c -o allocator

# Run the executable
./allocator
=======
# Custom C Memory Allocator (First-Fit & Coalescing)

A lightweight, custom dynamic memory allocator written in C. It implements core memory management concepts similar to `malloc` and `free` from the C standard library (`stdlib.h`) using a fixed-size heap buffer.

## Features

- **First-Fit Search**: Scans the heap sequentially to find the first block that accommodates the requested size.
- **Block Splitting**: Reduces internal fragmentation by dividing oversized free blocks into smaller allocations.
- **Adjacent Block Coalescing**: Merges neighboring free blocks during deallocation to prevent external fragmentation.
- **Pointer Arithmetic Safety**: Accurately computes block headers and metadata bounds.

## Memory Architecture

The allocator manages a `1024-byte` static array (`heap`) with the following block structure:
+-------------------+--------------------+
|  block_t Header   |    Payload Data    |
| (size, free flag) | (Allocated/Free)   |
+-------------------+--------------------+

- Each block starts with a header (`sizeof(block_t)`) containing metadata (`size` and `free` status).
- User pointers returned by `my_malloc` point directly to the **payload** area (just past the header).

## Getting Started

### Prerequisites

- Any standard C compiler (e.g., `gcc`, `clang`, or MSVC).

### Compilation & Execution

To compile and run the project using GCC:

```bash
# Compile the source code
gcc -Wall -Wextra -std=c99 main.c -o allocator

# Run the executable
./allocator
>>>>>>> 3ea1235 (Fix block alignment, metadata math and add heap trimming)
