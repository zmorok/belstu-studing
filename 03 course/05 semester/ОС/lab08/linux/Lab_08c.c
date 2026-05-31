#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>

typedef struct {
    void* region_start;
    size_t region_size;
    void** blocks;
    size_t block_count;
    size_t block_size;
} CustomHeap;

CustomHeap* heap_create(size_t min_size, size_t max_size) {
    void* region = mmap(NULL, max_size, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (region == MAP_FAILED) return NULL;
    CustomHeap* heap = malloc(sizeof(CustomHeap));
    if (!heap) { munmap(region, max_size); return NULL; }
    heap->region_start = region;
    heap->region_size = max_size;
    heap->blocks = NULL;
    heap->block_count = 0;
    heap->block_size = 0;
    if (mprotect(region, min_size, PROT_READ | PROT_WRITE) == -1) { free(heap); munmap(region, max_size); return NULL; }
    return heap;
}

void* heap_alloc(CustomHeap* heap, size_t size) {
    size_t committed = heap->region_size;
    void* block = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (block == MAP_FAILED) return NULL;
    heap->blocks = realloc(heap->blocks, (heap->block_count + 1) * sizeof(void*));
    heap->blocks[heap->block_count] = block;
    heap->block_count++;
    return block;
}

void heap_free(CustomHeap* heap, void* block) {
    for (size_t i = 0; i < heap->block_count; i++) {
        if (heap->blocks[i] == block) {
            munmap(block, heap->block_size);
            for (size_t j = i; j < heap->block_count - 1; j++) heap->blocks[j] = heap->blocks[j + 1];
            heap->block_count--;
            heap->blocks = realloc(heap->blocks, heap->block_count * sizeof(void*));
            break;
        }
    }
}

void heap_destroy(CustomHeap* heap) {
    if (!heap) return;    
    for (size_t i = 0; i < heap->block_count; i++) munmap(heap->blocks[i], heap->block_size);
    free(heap->blocks);    
    munmap(heap->region_start, heap->region_size);
    free(heap);
}

void show_memory_map() {
    char command[256];
    snprintf(command, sizeof(command), "pmap -X %d", getpid());
    printf("Memory map:\n");
    system(command);
}

int main() {
    printf("PID: %d\n\n", getpid());
    
    CustomHeap* heap = heap_create(1 * 1024 * 1024, 8 * 1024 * 1024);
    if (!heap) { printf("Failed to create heap\n"); return 1; }
    
    printf("Heap created. Press Enter to continue...\n");
    getchar();
    
    const size_t BLOCK_COUNT = 10;
    const size_t BLOCK_SIZE = 512 * 1024;
    
    printf("Allocating %zu blocks of %zu bytes...\n", BLOCK_COUNT, BLOCK_SIZE);
    
    void* blocks[BLOCK_COUNT];
    size_t allocated = 0;
    
    for (size_t i = 0; i < BLOCK_COUNT; i++) {
        blocks[i] = heap_alloc(heap, BLOCK_SIZE);
        if (!blocks[i]) { printf("Failed to allocate block #%zu\n", i); break; }
        allocated++;
        
        printf("Allocated block #%zu at %p\n", i + 1, blocks[i]);
        show_memory_map();
        printf("Press Enter to continue...\n");
        getchar();
    }
    
    printf("Filling blocks...\n");
    size_t intCount = BLOCK_SIZE / sizeof(int);
    for (size_t i = 0; i < allocated; i++) for (size_t j = 0; j < intCount; j++) ((int*)blocks[i])[j] = (int)j;
        
    printf("Freeing blocks...\n");
    for (size_t i = 0; i < allocated; i++) { heap_free(heap, blocks[i]); printf("Freed block #%zu\n", i + 1); }
    
    printf("Destroying heap...\n");
    heap_destroy(heap);
    
    printf("Done. Press Enter to exit...\n");
    getchar();
    
    return 0;
}