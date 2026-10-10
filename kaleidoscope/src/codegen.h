//
// Created by Sheldon Lobo on 2/15/26.
//

#ifndef KALEIDOSCOPE_CODEGEN_H
#define KALEIDOSCOPE_CODEGEN_H

#include <map>

#include <IR/IRBuilder.h>
#include <Target/TargetMachine.h>

extern std::unique_ptr<llvm::LLVMContext> TheContext;
extern std::unique_ptr<llvm::IRBuilder<>> Builder;
extern std::unique_ptr<llvm::Module> TheModule;
extern std::map<std::string, llvm::AllocaInst*> NamedValues;
extern std::unique_ptr<llvm::TargetMachine> TheTargetMachine;

extern void FunctionCodegen(std::unique_ptr<FunctionAST> FnAST);

extern llvm::AllocaInst* CreateEntryBlockAlloca(llvm::Function *theFunction, llvm::StringRef varName);

#endif //KALEIDOSCOPE_CODEGEN_H