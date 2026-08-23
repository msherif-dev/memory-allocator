# my-allocator

A custom heap memory allocator written from scratch in C, implementing the
core mechanics behind `malloc`/`free` — without using the standard library's
implementation.

Built as a learning project to understand how dynamic memory management
actually works at the system level: requesting memory from the OS, tracking
allocated/free blocks, reusing freed space, splitting, and merging adjacent
free blocks.

## Features (v1.0)

- **Custom `my_malloc` / `my_free`** — drop-in style API mirroring the
  standard allocator's interface.
- **`sbrk`-based heap growth** — memory is requested directly from the OS by
  moving the program break, the same low-level mechanism early Unix
  allocators relied on.
- **Block metadata tracking** — every allocation is preceded by a small
  header (`size`, `free` flag) used to walk the heap.
- **8-byte alignment** — all allocations are rounded up to the nearest
  multiple of 8, matching common ABI alignment requirements.
- **First-fit allocation strategy** — reuses the first free block large
  enough to satisfy a request, instead of always growing the heap.
- **Block splitting** — when a free block is larger than needed, it's split
  into a used portion and a smaller free remainder, reducing wasted space.
- **Block coalescing** — adjacent free blocks are merged back into a single
  larger block on every `free`, reducing fragmentation.
- **Pointer validation** — `my_free` rejects pointers that don't correspond
  to a valid block within the tracked heap range.
- **Double-free detection** — freeing an already-free block is caught and
  reported instead of silently corrupting heap state.
- **Heap introspection tools** — `heap_dump()` and `print_heap_status()`
  print the full block-by-block layout of the heap at any point, useful for
  debugging and for visually verifying allocator behavior.

## Project Structure

```
my-allocator/
├── include/
│   └── allocator.h        # Public API
├── src/
│   └── allocator.c        # Allocator implementation
├── tests/
│   └── test_allocator.c   # Manual test scenarios
├── Makefile
└── README.md
```

## Building & Running

Requires a C compiler (`gcc`) and a POSIX environment (`sbrk` is a POSIX/Unix
system call — this will not build as-is on Windows without WSL or a
compatibility layer).

```bash
make        # builds the test binary
make run    # builds and runs it
make clean  # removes the binary
```

## Example Output

Running the test suite allocates several blocks of different types, frees
one in the middle of the heap, allocates again to verify the freed space is
reused, and exercises the double-free guard:

```
===== AFTER FREE A =====
...
Block #0
Address : 0x...
Size    : 16
Status  : FREE
...

===== AFTER C =====
Was A's space reused? YES
```

## Design Notes

- **Metadata layout.** Each block is a `size_t size` + `int free` header
  immediately followed by the usable memory region. `metadata_size()`
  centralizes the (aligned) header size so every offset calculation in the
  allocator uses a single, consistent value.
- **First-fit vs. best-fit.** First-fit was chosen for v1 for simplicity and
  speed of implementation; it trades some memory efficiency for a much
  simpler, easier-to-verify search.
- **Coalescing runs on every free.** This keeps the implementation simple at
  the cost of doing a full heap walk (`O(n)`) on each `free` call — a
  known and accepted tradeoff for this version (see Roadmap).

## Known Limitations

- Coalescing and free-block search are both `O(n)` — every `malloc`/`free`
  may walk the entire heap. Fine for learning and small workloads, not
  representative of a production allocator's performance.
- No `realloc` or `calloc` yet.
- Not thread-safe — no locking around shared heap state.
- Memory obtained via `sbrk` is never returned to the OS (matches most
  simple allocators, including much of glibc's historical behavior for the
  main arena, but is worth knowing).

## Roadmap (v2 ideas)

- [ ] `my_realloc` and `my_calloc`
- [ ] Explicit free list (linked list of only free blocks) instead of
      scanning the full heap
- [ ] Boundary tags for O(1) coalescing on free
- [ ] Best-fit / segregated free lists as an allocation strategy comparison

## Why This Project

Understanding allocators means understanding how a huge amount of
lower-level systems programming works — from OS memory management to how
`malloc` implementations in real-world C programs behave under fragmentation
and reuse pressure. This project intentionally builds each mechanic
(alignment → metadata → first-fit → splitting → coalescing → validation)
as a separate, testable step rather than all at once.

## License

MIT — feel free to use, learn from, or extend this project.
