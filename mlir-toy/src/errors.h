//
// Created by Sheldon Lobo on 8/10/26.
//

#ifndef MLIR_TOY_SRC_ERRORS_H
#define MLIR_TOY_SRC_ERRORS_H

#include <memory>

#include "ast.h"

std::unique_ptr<ExprAST> LogError(const char *Str);
std::unique_ptr<PrototypeAST> LogErrorP(const char *Str);
llvm::Value *LogErrorV(const char *Str);

#endif // MLIR_TOY_SRC_ERRORS_H
