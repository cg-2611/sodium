#include "sodium/nac/parser/parser.h"

#include <memory>
#include <utility>
#include <unordered_set>
#include <vector>

#include "sodium/nac/ast/ast.h"
#include "sodium/nac/ast/ast_node.h"
#include "sodium/nac/ast/identifier.h"
#include "sodium/nac/ast/source_file.h"
#include "sodium/nac/ast/type.h"
#include "sodium/nac/errors/error_manager.h"
#include "sodium/nac/errors/parser_error.h"
#include "sodium/nac/lexer/token.h"

namespace sodium {

Parser::Parser(Token *token) : token_(token) {}

std::unique_ptr<AST> Parser::parse() {
    // a source file is the root of the AST
    std::unique_ptr<ASTNode> root(parseSourceFile());
    return std::make_unique<AST>(std::move(root));
}

std::unique_ptr<SourceFile> Parser::parseSourceFile() {
    std::vector<std::unique_ptr<Decl>> decls{};

    // parse declarations until we reach an EOF token
    while (token_->kind() != TokenKind::EOF_TOKEN) {
        decls.push_back(parseDecl());

        if (token_->kind() == TokenKind::EOF_TOKEN) {
            break;
        }

        advance();
    }

    return std::make_unique<SourceFile>(std::move(decls));
}

std::unique_ptr<Identifier> Parser::parseIdentifier() {
    if (token_->kind() != TokenKind::IDENTIFIER) {
        // expected identifier
        return nullptr;
    }

    return std::make_unique<Identifier>(token_->value());
}

std::unique_ptr<Type> Parser::parseType() {
    if (token_->kind() != TokenKind::TYPE) {
        // expected type
        return nullptr;
    }

    return std::make_unique<Type>(token_->value());
}

void Parser::advance() noexcept {
    token_ = token_->next();
}

Token *Parser::nextToken() const noexcept {
    return token_->next();
}

void Parser::synchronize(std::unordered_set<TokenKind> synchronizingTokens) {
    while (token_->kind() != TokenKind::EOF_TOKEN) {
        advance();

        if (synchronizingTokens.contains(token_->kind())) {
            return;
        }
    }
}

} // namespace sodium
