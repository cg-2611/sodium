#include "sodium/nac/main.h"

#include <iostream>

void sodium::nac::printHelloWorld() {
    std::cout << "Hello World!" << std::endl;
}

int main(int argc, char const *argv[]) {
    sodium::nac::printHelloWorld();
    return 0;
}
