#include "sodium/nac/nac.h"

#include <iostream>

#include "sodium/nac/lexer/lexer.h"
#include "sodium/nac/lexer/token.h"

namespace nac {

// used for debugging
static void printTokens(Token* token) {
    while (token != nullptr) {
        std::cout << "[nac]: " <<  token->kindString() << ": " << token->value() << "\n";
        token = token->next();
    }
}

void compileFile(File& file) {
    std::cout << "[nac]: tokenizing " << file.path() << "\n";

    Lexer lexer = Lexer(file.contents());

    std::unique_ptr<Token> token = lexer.tokenize();
    printTokens(token.get());

    std::cout << "[nac]: compilation complete\n";
}

} // namespace nac
