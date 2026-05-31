#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>

int main() {
    printf("pid: %d\n", getpid());

    long pageSize = sysconf(_SC_PAGESIZE);
    if (pageSize == -1) {perror("sysconf failed"); return 1; }
    printf("Page size: %ld bytes\n", pageSize);
    
    const size_t totalPages = 256;
    const size_t halfPages = totalPages / 2;
    const size_t totalSize = totalPages * pageSize;
    const size_t halfSize = halfPages * pageSize;

    printf("\n - 1: Reserve %zu pages (%zu bytes)\n", totalPages, totalSize);
    void* pMem = mmap(NULL, totalSize, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (pMem == MAP_FAILED) {perror("mmap reserve failed"); return 1; }
    printf("Reserved address: %p\n", pMem);
    printf("Press Enter to continue...\n");
    getchar();

    printf("\n - 2: Commit physical memory for second half (128 pages)\n");
    void* pCommit = (char*)pMem + halfSize;
    if (mprotect(pCommit, halfSize, PROT_READ | PROT_WRITE) == -1) {perror("mprotect failed"); munmap(pMem, totalSize); return 1; }
    printf("Committed address: %p\n", pCommit);
    printf("Press Enter to continue...\n");
    getchar();

    printf("\n - 3: Fill memory with integers\n");
    int* arr = (int*)pCommit;
    size_t intCount = halfSize / sizeof(int);
    for (size_t i = 0; i < intCount; ++i)  arr[i] = (int)i;
    printf("Filled %zu integers (0 to %zu)\n", intCount, intCount - 1);
    printf("Press Enter to continue...\n");
    getchar();

    printf("\n - 4: READONLY\n");
    if (mprotect(pCommit, halfSize, PROT_READ) == -1) perror("mprotect to READONLY failed");
    else printf("Protection changed to READONLY\n");
    printf("Press Enter to continue...\n");
    getchar();

    printf("\n - 5: Free 128 pages with array\n");
    if (munmap(pCommit, halfSize) == -1) perror("munmap second half failed");
    else printf("Second half unmapped successfully\n");
    printf("Press Enter to continue...\n");
    getchar();

    printf("\n - 6: Free remaining 128 pages\n");
    
    if (munmap(pMem, halfSize) == -1) perror("munmap first half failed");
    else printf("First half unmapped successfully\n");
    printf("All memory released. Press Enter to exit...\n");
    getchar();

    return 0;
}