#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

void HeapInfo(HANDLE heap)
{
    PROCESS_HEAP_ENTRY entry;
    SIZE_T totalSize = 0;

    printf("================ HEAP INFO ================\n");
    ZeroMemory(&entry, sizeof(entry));

    while (HeapWalk(heap, &entry))
    {
        printf("Address: %p | Size: %lu", entry.lpData, entry.cbData);
        totalSize += entry.cbData;
        printf(" | Type: ");

        if (entry.wFlags & PROCESS_HEAP_REGION) printf("REGION ");
        if (entry.wFlags & PROCESS_HEAP_UNCOMMITTED_RANGE) printf("UNCOMMITTED ");
        if (entry.wFlags & PROCESS_HEAP_ENTRY_BUSY) printf("BUSY ");
        if (entry.wFlags & PROCESS_HEAP_ENTRY_MOVEABLE) printf("MOVEABLE ");
        if (entry.wFlags & PROCESS_HEAP_ENTRY_DDESHARE) printf("DDESHARE ");
        printf("\n");
    }

    printf("===========================================\n");
    printf("Total size of used regions: %llu byte\n", (unsigned long long)totalSize);
    printf("===========================================\n\n");
}

int main()
{
    SIZE_T minSize = 1ULL * 1024 * 1024, maxSize = 8ULL * 1024 * 1024;
    HANDLE heap = HeapCreate(0, minSize, maxSize);
    if (!heap) { printf("Ex HeapCreate: %lu\n", GetLastError()); return 1; }

    HeapInfo(heap);
    system("pause");
    system("cls");

    const size_t BLOCK_COUNT = 10;
    const size_t BLOCK_SIZE = 512 * 1024;

    int** blocks = (int**)malloc(BLOCK_COUNT * sizeof(int*));
    if (!blocks) { printf("Ex malloc\n"); HeapDestroy(heap); return 1; }

    int allocated_count = 0;

    printf("Allocating %zu blocks of %zu byte...\n", BLOCK_COUNT, BLOCK_SIZE);

    for (size_t i = 0; i < BLOCK_COUNT; i++)
    {
        blocks[i] = (int*)HeapAlloc(heap, HEAP_ZERO_MEMORY, BLOCK_SIZE);
        if (!blocks[i]) { printf("Ex HeapAlloc %zu : %lu\n", i, GetLastError()); break; }

        allocated_count++;
        printf("Allocated block #%zu\n", i + 1);

        HeapInfo(heap);
        system("pause & cls");
    }

    printf("Filling arrays...\n");
    size_t intCount = BLOCK_SIZE / sizeof(int);
    for (size_t i = 0; i < allocated_count; i++) for (size_t j = 0; j < intCount; j++) blocks[i][j] = (int)j;
    
    printf("Free blocks...\n");
    for (size_t i = 0; i < allocated_count; i++) if (blocks[i]) HeapFree(heap, 0, blocks[i]);
        
    free(blocks);
    HeapInfo(heap);
    system("pause & cls");

    printf("Free heap...\n");
    HeapDestroy(heap);
    printf("Heap is free.\n");

    return 0;
}