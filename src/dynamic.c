
#include <stdio.h>

void __declspec(dllexport) dynamic_print_stuff() {
    printf("Hello from the dynamic lib\n");
}
