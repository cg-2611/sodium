#include "sodium/nac/codegen/target.h"

#include <string>
#include <system_error>

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"

namespace sodium {

Target::Target(llvm::Module *module) : module_(module) {}

void Target::generate_object_code() const {
    auto target_triple = llvm::sys::getDefaultTargetTriple();

    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();

    auto error = std::string();
    const auto *target = llvm::TargetRegistry::lookupTarget(target_triple, error);

    if (!target) {
        llvm::errs() << error;
        return;
    }

    auto cpu = std::string("generic");
    auto features = std::string();

    auto target_options = llvm::TargetOptions();
    auto reloc_model = llvm::Optional<llvm::Reloc::Model>();
    auto *target_machine = target->createTargetMachine(target_triple, cpu, features, target_options, reloc_model);

    module_->setDataLayout(target_machine->createDataLayout());
    module_->setTargetTriple(target_triple);

    auto file_name = std::string("main.o");
    auto ec = std::error_code();
    auto dest = llvm::raw_fd_ostream(file_name, ec, llvm::sys::fs::OF_None);

    if (ec) {
        llvm::errs() << "could not open file: " << ec.message() << '\n';
        return;
    }

    auto pass_manager = llvm::legacy::PassManager();
    auto failure = target_machine->addPassesToEmitFile(pass_manager, dest, nullptr, llvm::CGFT_ObjectFile);

    if (failure) {
        llvm::errs() << "targetMachine can't emit a file of this type\n";
        return;
    }

    pass_manager.run(*module_);
    dest.flush();

    // module_->print(llvm::outs(), nullptr);
}

} // namespace sodium
