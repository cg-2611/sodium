#include "sodium/nac/nac.h"

#include "sodium/nac/ast/ast.h"
#include "sodium/nac/ast/ast_printer.h"
#include "sodium/nac/basic/file.h"
#include "sodium/nac/errors/error_manager.h"
#include "sodium/nac/exceptions/exception.h"
#include "sodium/nac/lexer/lexer.h"
#include "sodium/nac/parser/parser.h"
#include "sodium/nac/token/token_buffer.h"

#include <iostream>

namespace sodium {

void compile_file([[maybe_unused]] const File &file) {
    auto lexer = Lexer(file);
    auto tokens = lexer.tokenize();

    auto parser = Parser(tokens);
    auto ast = parser.parse();

    // if errors have been encountered during parsing, throw an exception
    if (ErrorManager::has_errors()) {
        throw Exception(ExceptionKind::ERRORS_GENERATED);
    }

    // temporary for debugging
    ASTPrinter().print_ast(ast);
}

} // namespace sodium
