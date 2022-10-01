#include "sodium/nac/nac.h"

#include "sodium/nac/ast/ast.h"
#include "sodium/nac/ast/ast_printer.h"
#include "sodium/nac/basic/file.h"
#include "sodium/nac/codegen/codegen.h"
#include "sodium/nac/diagnostics/diagnostic_engine.h"
#include "sodium/nac/lexer/lexer.h"
#include "sodium/nac/parser/parser.h"
#include "sodium/nac/token/token_buffer.h"

namespace sodium {

bool compile_file(const File &file) {
    auto lexer = Lexer(file.contents(), file.diagnostics());
    auto tokens = lexer.tokenize();

    auto parser = Parser(tokens, file.diagnostics());
    auto ast = parser.parse();

    auto codegen = Codegen(ast);
    codegen.generate();
    codegen.print_llvm_ir(); // for debugging

    return file.diagnostics().has_problems();
}

} // namespace sodium
