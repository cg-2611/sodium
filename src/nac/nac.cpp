#include "sodium/nac/nac.h"

#include <iostream>
#include <memory>

#include "sodium/nac/ast/ast.h"
#include "sodium/nac/ast/ast_printer.h"
#include "sodium/nac/errors/error_manager.h"
#include "sodium/nac/exceptions/exception.h"
#include "sodium/nac/io/file.h"
#include "sodium/nac/lexer/lexer.h"
#include "sodium/nac/lexer/token.h"
#include "sodium/nac/parser/parser.h"

namespace sodium {

void compileFile(const File &file) {
    Lexer lexer(file.contents());
    std::unique_ptr<Token> token(lexer.tokenize());

    // if errors have been encountered during tokenizing, throw an exception
    if (ErrorManager::hasErrors()) {
        throw Exception(ExceptionKind::ERRORS_GENERATED);
    }

    Parser parser(token.get());
    std::unique_ptr<AST> ast(parser.parse());

    // if errors have been encountered during tokenizing, throw an exception
    if (ErrorManager::hasErrors()) {
        throw Exception(ExceptionKind::ERRORS_GENERATED);
    }

    // temporary for debugging
    ASTPrinter(2).printAST(ast.get());

    std::cout << "[nac]: compilation complete\n";
}

} // namespace sodium
