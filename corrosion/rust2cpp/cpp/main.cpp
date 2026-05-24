#include <iostream>

#include "rust_lib/rust_lib.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        rust_function("Cpp");
    } else {
        rust_function(argv[1]);
    }
    std::cout << "add(2, 2) = " << add(2, 2) << std::endl;
    #ifdef NDEBUG
    printf("Release configuration!\n");
    #else
    printf("Debug configuration!\n");
    #endif
    return 0;
}
