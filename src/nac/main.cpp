#include <iostream>

#include "sodium/nac/nac.h"

int main(int argc, const char *argv[]) {
    if (argc != 2) {
        std::cerr << "invalid arguments passed\n";
        return EXIT_FAILURE;
    }

    try {
        sodium::nac::compileFile(argv[1]);
    } catch(const std::exception &e) {
        std::cerr << e.what() << "\n";
    }

    return EXIT_SUCCESS;
}
