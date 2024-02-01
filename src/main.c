
#include <stdio.h>
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

int main(int argc, char *argv[]) {
    printf("Hello, World!\n");

    void (*target)(void) = 0;

    HMODULE dylib = NULL;
    
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

        if (dylib != NULL) {
            FreeLibrary(dylib);
        }

        dylib = LoadLibrary(TEXT("dynamic"));
        if (dylib == NULL) {
            printf("Failed loading dynamic lib\n");
            return 1;
        }

        target = (void(*)(void)) GetProcAddress(dylib, "dynamic_print_stuff");
        if (target == NULL) {
            printf("Failed finding dynamic function\n");
            return 1;
        }

        target();

    } while (running);

    FreeLibrary(dylib);
    return 0;
}
