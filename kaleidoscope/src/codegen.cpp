//
// Created by Sheldon Lobo on 2/15/26.
//

#include <map>
#include <memory>

#include "ast.h"

#include "IR/IRBuilder.h"
#include "IR/Module.h"

std::unique_ptr<llvm::LLVMContext> TheContext;
std::unique_ptr<llvm::IRBuilder<>> Builder;
std::unique_ptr<llvm::Module> TheModule;
std::map<std::string, llvm::Value *> NamedValues;
