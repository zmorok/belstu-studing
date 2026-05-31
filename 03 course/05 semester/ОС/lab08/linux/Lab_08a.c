#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int g_init = 42;
int g_uninit;

static int g_s_init = 10;
static int g_s_uninit;

void myFunction() {
    int l_init = 5;
    int l_uninit;

    static int l_s_init = 7;
    static int l_s_uninit;

    printf("Address of myFunction:         %p\n", (void*)myFunction);
    printf("Address of l_init:             %p\n", (void*)&l_init);
    printf("Address of l_uninit:           %p\n", (void*)&l_uninit);
    printf("Address of l_s_init:           %p\n", (void*)&l_s_init);
    printf("Address of l_s_uninit:         %p\n", (void*)&l_s_uninit);
}

int main(int argc, char* argv[]) {
    printf("pid: %d\n", getpid());
    printf("Address of main:               %p\n", (void*)main);
    printf("Global initialized:            %p\n", (void*)&g_init);
    printf("Global uninitialized:          %p\n", (void*)&g_uninit);
    printf("Static global initialized:     %p\n", (void*)&g_s_init);
    printf("Static global uninitialized:   %p\n", (void*)&g_s_uninit);
    printf("Address of argc:               %p\n", (void*)&argc);
    printf("Address of argv:               %p\n", (void*)&argv);
    printf("Address of argv[0] (program):  %p\n", (void*)argv[0]);

    myFunction();

    printf("Press Enter to exit...\n");
    getchar();

    return 0;
}