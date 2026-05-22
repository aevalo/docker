#include <iostream>

extern "C" void rust_function(char const *name);
extern "C" int add(int left, int right);

int main(int argc, char **argv) {
    if (argc < 2) {
        rust_function("Cpp");
    } else {
        rust_function(argv[1]);
    }
    std::cout << "add(2, 2) = " << add(2, 2) << std::endl;
    return 0;
}
