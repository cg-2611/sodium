#include <iostream>

#include "sodium/nac/io/file.h"
#include "sodium/nac/nac.h"

int main(int argc, const char *argv[]) {
    if (argc != 2) {
        std::cerr << "invalid arguments passed\n";
        return EXIT_FAILURE;
    }

    try {
        sodium::File file(argv[1]);
        sodium::compile_file(file);
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
