#include "sodium/nac/nac.h"

#include <iostream>
#include <vector>

#include "sodium/nac/lexer/lexer.h"
#include "sodium/nac/lexer/token.h"

void nac::compileFile(File &file) {
    std::cout << "[nac]: tokenizing " << file.getPath() << "\n";

    nac::Lexer lexer = nac::Lexer(file.getContents());
    std::vector<nac::Token> tokens = lexer.tokenize();

    for (nac::Token &token : tokens) {
        std::cout << "[nac]: " <<  token.getKindAsString() << ": " << token.getValue() << "\n";
    }

    std::cout << "[nac]: compilation complete\n";
}
