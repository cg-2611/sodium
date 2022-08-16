#include "sodium/nac/ast/parameter_list.h"

#include <memory>
#include <utility>
#include <vector>

#include "sodium/nac/ast/ast_node.h"
#include "sodium/nac/ast/ast_visitor.h"
#include "sodium/nac/ast/identifier.h"

namespace sodium {

Parameter::Parameter(std::unique_ptr<Identifier> identifier)
        : ASTNode(ASTNodeKind::PARAMETER),
          identifier_(std::move(identifier)) {}

void Parameter::accept(ASTVisitor *visitor) const {
    identifier_->accept(visitor);
    visitor->visit(this);
}

Identifier *Parameter::identifier() const noexcept {
    return identifier_.get();
}

ParameterList::ParameterList(std::vector<std::unique_ptr<Parameter>> parameters)
        : ASTNode(ASTNodeKind::PARAMETER_LIST),
          parameters_(std::move(parameters)) {}

void ParameterList::accept(ASTVisitor *visitor) const {
    // call the accept method of all the parameters in the list
    for (auto &&parameter : parameters_) {
        parameter->accept(visitor);
    }

    visitor->visit(this);
}

const std::vector<std::unique_ptr<Parameter>> &ParameterList::parameters() const noexcept {
    return parameters_;
}

} // namespace sodium
