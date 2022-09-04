#include "sodium/nac/nac.h"

#include <iostream>
#include <memory>

#include "sodium/nac/ast/ast.h"
#include "sodium/nac/ast/ast_printer.h"
#include "sodium/nac/errors/error_manager.h"
#include "sodium/nac/exceptions/exception.h"
#include "sodium/nac/io/file.h"
#include "sodium/nac/parser/parser.h"

#include "sodium/nac/lexer/lexer.h"

namespace sodium {

void compileFile(const File &file) {
    Parser parser(file.contents());
    std::unique_ptr<AST> ast(parser.parse());

    // if errors have been encountered during parsing, throw an exception
    if (ErrorManager::hasErrors()) {
        throw Exception(ExceptionKind::ERRORS_GENERATED);
    }

    // temporary for debugging
    ASTPrinter(2).printAST(ast.get());

    std::cout << "[nac]: compilation complete\n";
}

} // namespace sodium
