
#include <stdio.h>

/*
before reloading:
rename the old dynamic lib
recompile source -> dylib
*/

void __declspec(dllexport) dynamic_print_stuff(void) {
    printf("Testing the library - go!\n");
}
