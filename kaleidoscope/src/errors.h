//
// Created by Sheldon Lobo on 2/15/26.
//

#ifndef KALEIDOSCOPE_ERRORS_H
#define KALEIDOSCOPE_ERRORS_H

#include <memory>

#include "ast.h"

std::unique_ptr<ExprAST> LogError(const char *Str);
std::unique_ptr<PrototypeAST> LogErrorP(const char *Str);
llvm::Value *LogErrorV(const char *Str);

#endif //KALEIDOSCOPE_ERRORS_H