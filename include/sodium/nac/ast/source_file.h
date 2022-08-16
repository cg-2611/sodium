#ifndef SODIUM_NAC_AST_SOURCE_FILE_H
#define SODIUM_NAC_AST_SOURCE_FILE_H

#include <memory>
#include <vector>

#include "sodium/nac/ast/ast_node.h"

namespace sodium {

class ASTVisitor;
class Decl;

/**
 * @brief A derived class of ASTNode, used to represent a Sodium source file.
 *
 */
class SourceFile : public ASTNode {
public:
    /**
     * @brief Construct a new SourceFile object.
     *
     * @param decls An std::vector<std::unique_ptr<Decl>> that are all the declarations in the source file.
     */
    SourceFile(std::vector<std::unique_ptr<Decl>> decls);

    /**
     * @brief Destroy the SourceFile object.
     *
     */
    ~SourceFile() = default;

    /**
     * @brief Used to accept, for a source file node, aASTVisitor derived class for use with the visitor pattern.
     *        It calls the accept method of each declaration in the vector of declarations.
     *
     * @param visitor The visitor object being accepted by this source file node.
     */
    void accept(ASTVisitor *visitor) const override;

    /**
     * @brief Accessor for the list of declarations in the source file.
     *
     * @return const std::vector<std::unique_ptr<Decl>>& that stores the declarations in the list.
     */
    const std::vector<std::unique_ptr<Decl>> &decls() const noexcept;

private:
    std::vector<std::unique_ptr<Decl>> decls_;
};

} // namespace sodium

#endif // SODIUM_NAC_AST_SOURCE_FILE_H
