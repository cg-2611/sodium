#include <iostream>

#include "sodium/nac/error/error.h"
#include "sodium/nac/io/file.h"
#include "sodium/nac/nac.h"

int main(int argc, const char *argv[]) {
    if (argc != 2) {
        std::cerr << "invalid arguments passed\n";
        return EXIT_FAILURE;
    }

    try {
        nac::File file(argv[1]);
        nac::compileFile(file);
    } catch(const nac::Exception &e) {
        std::cerr << e.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
