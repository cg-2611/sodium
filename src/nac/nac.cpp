#include "sodium/nac/nac.h"

#include <iostream>

void sodium::nac::compileFile(const char *filePath) {
    sodium::nac::File file = sodium::nac::File(filePath);

    std::cout << file.getPath() << ": " << file.getSize() << "\n";
    std::cout << file.getContents() << "\n";
}
