#include "sodium/nac/nac.h"

#include <iostream>

#include "sodium/nac/lexer/lexer.h"
#include "sodium/nac/lexer/token.h"

namespace nac {

// used for debugging
static constexpr std::string_view kindString(TokenKind tokenKind) {
    switch (tokenKind) {
        case TokenKind::TOKEN_EOF: return "TOKEN_EOF";
        case TokenKind::TOKEN_KEYWORD: return "TOKEN_KEYWORD";
        case TokenKind::TOKEN_TYPE: return "TOKEN_TYPE";
        case TokenKind::TOKEN_IDENTIFIER: return "TOKEN_IDENTIFIER";
        case TokenKind::TOKEN_NUMERIC_LITERAL: return "TOKEN_NUMERIC_LITERAL";
        case TokenKind::TOKEN_COLON: return "TOKEN_COLON";
        case TokenKind::TOKEN_LEFT_BRACE: return "TOKEN_LEFT_BRACE";
        case TokenKind::TOKEN_LEFT_PAREN: return "TOKEN_LEFT_PAREN";
        case TokenKind::TOKEN_RIGHT_BRACE: return "TOKEN_RIGHT_BRACE";
        case TokenKind::TOKEN_RIGHT_PAREN: return "TOKEN_RIGHT_PAREN";
        case TokenKind::TOKEN_SEMI_COLON: return "TOKEN_SEMI_COLON";
        default: return "UNRECOGNIZED_TOKEN";
    }
}

// used for debugging
static void printTokens(Token* token) {
    while (token != nullptr) {
        std::cout << "[nac]: " << kindString(token->kind()) << ": " << token->value() << '\n';
        token = token->next();
    }
}

void compileFile(File& file) {
    Lexer lexer = Lexer(file.contents());

    std::unique_ptr<Token> token = lexer.tokenize();
    printTokens(token.get());

    std::cout << "[nac]: compilation complete\n";
}

} // namespace nac
