#include "sodium/nac/nac.h"

#include <sys/wait.h>
#include <unistd.h>

#include "sodium/nac/ast/ast.h"
#include "sodium/nac/ast/ast_printer.h"
#include "sodium/nac/basic/file.h"
#include "sodium/nac/codegen/codegen.h"
#include "sodium/nac/codegen/target.h"
#include "sodium/nac/diagnostics/diagnostic_engine.h"
#include "sodium/nac/lexer/lexer.h"
#include "sodium/nac/parser/parser.h"
#include "sodium/nac/token/token_buffer.h"

namespace sodium {

static bool create_executable();

bool compile_file(const File &file) {
    auto lexer = Lexer(file.contents(), file.diagnostics());
    auto tokens = lexer.tokenize();

    auto parser = Parser(tokens, file.diagnostics());
    auto ast = parser.parse();

    if (file.diagnostics().has_problems()) {
        return false;
    }

    auto codegen = Codegen(ast, file.diagnostics());
    auto *llvm_module = codegen.generate();

    auto target = Target(llvm_module);
    target.generate_object_code();

    if (file.diagnostics().has_problems() || !create_executable()) {
        return false;
    }

    return file.diagnostics().has_problems();
}

static bool create_executable() {
    auto fork_status = fork();
    if (fork_status == -1) {
        return false;
    }

    if (fork_status == 0) {
        // this is executed by the child process
        auto exec_status = execlp("clang", "clang", "-o", "main", "main.o", NULL);
        if (exec_status == -1) {
            // error creating executable
            return false;
        }
    } else {
        // this is executed by the parent process
        auto wait_status = -1;
        wait(&wait_status);
        auto exit_code = WIFEXITED(wait_status);
        if (exit_code != 0) {
            // wait for child process failed
            return false;
        }
    }

    return true;
}

} // namespace sodium
