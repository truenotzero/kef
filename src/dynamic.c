#include <khot.h>
#include <stdio.h>

/*
before reloading:
rename the old dynamic lib
recompile source -> dylib
*/

KDYFUN void dynamic_print_stuff(void) {
    printf("Testing the library - go!\n");
}
