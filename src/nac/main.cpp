#include <cstdlib>
#include <iostream>

#include "sodium/nac/basic/file.h"
#include "sodium/nac/diagnostics/diagnostic_engine.h"
#include "sodium/nac/nac.h"

int main(int argc, const char *argv[]) {
    if (argc != 2) {
        std::cerr << "invalid arguments passed\n";
        return EXIT_FAILURE;
    }

    auto file = sodium::File(argv[1]);
    if (file.diagnostics().has_problems()) {
        file.diagnostics().emit_diagnostics(std::cerr);
        return EXIT_FAILURE;
    }

    auto problems = sodium::compile_file(file);
    if (problems) {
        file.diagnostics().emit_diagnostics(std::cerr);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
