//
// Created by Sheldon Lobo on 2/15/26.
//

#include <map>
#include <memory>
#include <iostream>

#include "ast.h"
#include "main.h"
#include "parser.h"

#include "IR/IRBuilder.h"
#include "IR/Module.h"

std::unique_ptr<llvm::LLVMContext> TheContext;
std::unique_ptr<llvm::IRBuilder<>> Builder;
std::unique_ptr<llvm::Module> TheModule;
std::map<std::string, llvm::Value *> NamedValues;

//===----------------------------------------------------------------------===//
// Codegen
//===----------------------------------------------------------------------===//

void FunctionCodegen(std::unique_ptr<FunctionAST> FnAST) {
    if (!FnAST)
        return;

    if (auto *FnIR = FnAST->codegen()) {
        if (debug) {
            std::cerr << "Function code generated:\n";
            FnIR->print(llvm::errs());
            std::cerr << std::endl;
        }
    } else {
        // Skip token for error recovery.
        getNextToken();
    }
}
