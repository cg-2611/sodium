#include "sodium/nac/nac.h"

#include <iostream>
#include <string_view>

#include "sodium/nac/errors/error_manager.h"
#include "sodium/nac/exceptions/exception.h"
#include "sodium/nac/lexer/lexer.h"
#include "sodium/nac/lexer/token.h"

namespace sodium {

// used for debugging
static constexpr std::string_view kindString(TokenKind tokenKind) {
    switch (tokenKind) {
        case TokenKind::KEYWORD: return "KEYWORD";
        case TokenKind::TYPE: return "TYPE";
        case TokenKind::IDENTIFIER: return "IDENTIFIER";
        case TokenKind::NUMERIC_LITERAL: return "NUMERIC_LITERAL";
        case TokenKind::LEFT_BRACE: return "LEFT_BRACE";
        case TokenKind::RIGHT_BRACE: return "RIGHT_BRACE";
        case TokenKind::LEFT_PAREN: return "LEFT_PAREN";
        case TokenKind::RIGHT_PAREN: return "RIGHT_PAREN";
        case TokenKind::ARROW: return "ARROW";
        case TokenKind::EOF_TOKEN: return "EOF_TOKEN";
        case TokenKind::EOL_TOKEN: return "EOL_TOKEN";
        case TokenKind::ERROR_TOKEN: return "ERROR_TOKEN";
        default: return "UNRECOGNIZED_TOKEN";
    }
}

// used for debugging
static void printTokens(Token *token) {
    while (token != nullptr) {
        std::cout << "[nac]: " << kindString(token->kind()) << ": " << token->value() << '\n';
        token = token->next();
    }
}

void compileFile(File &file) {
    Lexer lexer(file.contents());

    std::unique_ptr<Token> token(lexer.tokenize());

    // if errors have been encountered during tokenizing, throw an exception
    if (ErrorManager::hasErrors()) {
        throw Exception(ExceptionKind::ERRORS_GENERATED);
    }

    printTokens(token.get());

    std::cout << "[nac]: compilation complete\n";
}

} // namespace sodium
