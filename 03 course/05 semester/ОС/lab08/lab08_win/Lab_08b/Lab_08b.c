#include <windows.h>
#include <stdio.h>

int main() {
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    DWORD pageSize = sysInfo.dwPageSize;
    printf("Page size: %lu bytes\n", pageSize);

    const size_t totalPages = 256;
    const size_t halfPages = totalPages / 2;

    printf("\n - 1: Reserve 256 pages\n");
    LPBYTE pMem = (LPBYTE)VirtualAlloc(NULL, totalPages * pageSize, MEM_RESERVE, PAGE_READWRITE);

    if (!pMem) fprintf(stderr, "Failed to reserve memory!\n"); return 1;

    printf("Reserved address: %p\n", pMem);
    printf("Press Enter...\n");
    getchar();

    printf("\n - 2: Second half = 128 pages\n");
    LPBYTE pCommit = pMem + halfPages * pageSize;
    if (!VirtualAlloc(pCommit, halfPages * pageSize, MEM_COMMIT, PAGE_READWRITE)) fprintf(stderr, "Failed to commit memory!\n"); return 1;

    printf("Committed address: %p\n", pCommit);
    printf("Press Enter...\n");
    getchar();

    printf("\n - 3: Fill physical memory with integers\n");
    int* arr = (int*)pCommit;
    size_t intCount = (halfPages * pageSize) / sizeof(int);
    for (size_t i = 0; i < intCount; ++i) arr[i] = (int)i;
    printf("Memory filled with integers 0..%zu\n", intCount - 1);
    printf("Press Enter...\n");
    getchar();

    printf("\n - 4: READONLY\n");
    DWORD oldProtect;
    if (!VirtualProtect(pCommit, halfPages * pageSize, PAGE_READONLY, &oldProtect)) fprintf(stderr, "Failed to change protection!\n");
    else printf("Protection changed to READONLY\n");
    
    printf("Press Enter...\n");
    getchar();

    printf("\n - 5: Free physical memory ===\n");
    if (!VirtualFree(pCommit, halfPages * pageSize, MEM_DECOMMIT)) fprintf(stderr, "Failed to decommit memory!\n");
    else printf("Decommitted second half\n");
    printf("Press Enter...\n");
    getchar();

    printf("\n - 6: Free all reserved memory\n");
    if (!VirtualFree(pMem, 0, MEM_RELEASE)) fprintf(stderr, "Failed to release memory!\n");
    else printf("All reserved memory released\n");
    printf("Press Enter to exit...\n");
    getchar();

    return 0;
}