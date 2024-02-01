
#include <stdio.h>
#include "core/hotreload.h"

int main(int argc, char *argv[]) {
    (void) argc;
    (void) argv;
    
    printf("Hello, World!\n");

    void (*target)(void) = 0;

    kDylib dylib;
    
    int running = 1;
    do {
        int input = getchar();
        switch (input) {
            case 'r':
            case 'R':
                break;
            
            case 'q':
            case 'Q':
                running = 0;
                // fall through
            default:
                continue;
        }


        if (!kLoadDylib(&dylib, "dynamic")) {
            printf("Failed to load dynamic lib\n");
            return 1;
        }

        if (!kGetDyfun(dylib, "dynamic_print_stuff", (void **) &target)) {
            printf("Failed to load dynamic fun\n");
            return 1;
        }

        target();

    } while (running);

    kFreeDylib(&dylib);
    return 0;
}
