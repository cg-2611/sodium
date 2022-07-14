#include "sodium/nac/nac.h"

#include <iostream>

#include "sodium/nac/error/error.h"
#include "sodium/nac/io/file.h"

void sodium::nac::compileFile(const std::string &filePath) {
    try {
        sodium::nac::File file = sodium::nac::File(filePath);

        std::cout << file.getPath() << ": " << file.getSize() << "\n";
        std::cout << file.getContents() << "\n";
    } catch (const std::exception &e) {
        std::cerr << e.what() << "\n";
    }
}
