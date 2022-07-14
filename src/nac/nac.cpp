#include "sodium/nac/nac.h"

#include <iostream>

#include "sodium/nac/error/error.h"
#include "sodium/nac/io/file.h"
#include "sodium/nac/lexer/token.h"
#include "sodium/nac/lexer/lexer.h"

void sodium::nac::compileFile(const std::string &filePath) {
    try {
        sodium::nac::File file = sodium::nac::File(filePath);
        std::cout << file.getPath() << ": " << file.getSize() << "\n";

        sodium::nac::Lexer lexer = sodium::nac::Lexer(file);
        std::shared_ptr<sodium::nac::Token> token = lexer.tokenize();

        while (token->getKind() != sodium::nac::TokenKind::TOKEN_EOF) {
            std::cout << "[nac]: " << token->getKindAsString() << ": " << token->getValue() << "\n";

            token = token->getNextToken();
        }

        std::cout << "[nac]: compilation complete\n";
    } catch (const std::exception &e) {
        std::cerr << e.what() << "\n";
    }
}
