
#include <stdio.h>

/*
before reloading:
rename the old dynamic lib
recompile source -> dylib
*/

void __declspec(dllexport) dynamic_print_stuff() {
    printf("Dynamic hot reloading - got the concepts down!");
}
