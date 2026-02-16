//
// Created by Sheldon Lobo on 2/15/26.
//

#include <iostream>

#include "ast.h"

/// LogError* - These are little helper functions for error handling.
std::unique_ptr<ExprAST> LogError(const char *Str) {
    std::cerr << "Error: %s" << Str << std::endl;
    return nullptr;
}

std::unique_ptr<PrototypeAST> LogErrorP(const char *Str) {
    LogError(Str);
    return nullptr;
}

llvm::Value *LogErrorV(const char *Str) {
    LogError(Str);
    return nullptr;
}
