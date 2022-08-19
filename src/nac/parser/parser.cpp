#include "sodium/nac/parser/parser.h"

#include <memory>
#include <utility>
#include <vector>

#include "sodium/nac/ast/ast.h"
#include "sodium/nac/ast/ast_node.h"
#include "sodium/nac/ast/identifier.h"
#include "sodium/nac/ast/source_file.h"
#include "sodium/nac/ast/type.h"
#include "sodium/nac/lexer/token.h"

namespace sodium {

Parser::Parser(Token *token) : token_(token) {}

std::unique_ptr<AST> Parser::parse() {
    // a source file is the root of the AST
    std::unique_ptr<ASTNode> root(parseSourceFile());
    return std::make_unique<AST>(std::move(root));
}

// SourceFile = Declaration* EOF ;
std::unique_ptr<SourceFile> Parser::parseSourceFile() {
    std::vector<std::unique_ptr<Decl>> decls{};

    // parse declarations until we reach an EOF token
    while (token_->kind() != TokenKind::EOF_TOKEN) {
        decls.push_back(parseDecl());

        if (token_->kind() == TokenKind::EOF_TOKEN) {
            break;
        }

        skipExcessEOLTokens(); // skip excess EOL tokens after declaration

        advance();
    }

    return std::make_unique<SourceFile>(std::move(decls));
}

// Identifier = IdentifierHead IdentifierCharacter* ;
// IdentifierHead = "_" | LETTER ;
// IdentifierCharacter = IdentifierHead | DIGIT ;
std::unique_ptr<Identifier> Parser::parseIdentifier() {
    if (token_->kind() != TokenKind::IDENTIFIER) {
        // expected an identifier
    }

    return std::make_unique<Identifier>(token_->value());
}

// Type = int ;
std::unique_ptr<Type> Parser::parseType() {
    if (token_->kind() != TokenKind::TYPE) {
        // expected a type
    }

    return std::make_unique<Type>(token_->value());
}

void Parser::skipExcessEOLTokens() {
    while (nextToken()->kind() == TokenKind::EOL_TOKEN) {
        advance();
    }
}

void Parser::advance() noexcept {
    token_ = token_->next();
}

Token *Parser::nextToken() const noexcept {
    return token_->next();
}

} // namespace sodium
