#ifndef SODIUM_NAC_TARGET_TARGET_H
#define SODIUM_NAC_TARGET_TARGET_H

namespace llvm {

class Module;

} // namespace llvm

namespace sodium {

/// Used to generate an object file from an llvm::Module.
class Target {
public:
    /// Constructor for Target.
    /// \param module The module to generate the object code for.
    Target(llvm::Module *module);

    /// Perform the object code generation.
    void generate_object_code() const;

private:
    llvm::Module *module_;
};

} // namespace sodium

#endif // SODIUM_NAC_TARGET_TARGET_H
