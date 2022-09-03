#include "sodium/nac/ast/source_file.h"

#include <memory>
#include <utility>
#include <vector>

#include "sodium/nac/ast/ast_node.h"
#include "sodium/nac/ast/ast_visitor.h"
#include "sodium/nac/ast/decl.h"

namespace sodium {

SourceFile::SourceFile(std::vector<std::unique_ptr<Decl>> decls)
        : ASTNode(ASTNodeKind::SOURCE_FILE),
          decls_(std::move(decls)) {}

void SourceFile::accept(ASTVisitor *visitor) const {
    // call the accept method of all the declarations in the list
    for (auto &&decl : decls_) {
        decl->accept(visitor);
    }

    visitor->visit(this);
}

const std::vector<std::unique_ptr<Decl>> &SourceFile::decls() const noexcept {
    return decls_;
}

} // namespace sodium