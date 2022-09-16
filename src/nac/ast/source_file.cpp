#include "sodium/nac/ast/source_file.h"

#include <memory>
#include <utility>
#include <vector>

#include "sodium/nac/ast/ast_node.h"
#include "sodium/nac/ast/ast_visitor.h"
#include "sodium/nac/ast/decl.h"
#include "sodium/nac/basic/source_range.h"

namespace sodium {

SourceFile::SourceFile(std::vector<std::unique_ptr<Decl>> decls, SourceRange range)
        : ASTNode(ASTNodeKind::SOURCE_FILE, range),
          decls_(std::move(decls)) {}

void SourceFile::accept(ASTVisitor &visitor) const {
    visitor.visit(*this);
}

const std::vector<std::unique_ptr<Decl>> &SourceFile::decls() const {
    return decls_;
}

} // namespace sodium
