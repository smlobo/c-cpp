//
// Created by Sheldon Lobo on 8/10/26.
//

#ifndef MLIR_TOY_SRC_PARSER_H
#define MLIR_TOY_SRC_PARSER_H

#include <map>
#include <memory>

#include "ast.h"

extern int CurToken;
extern int getNextToken();

extern std::unique_ptr<FunctionAST> HandleDefinition();
extern void HandleExtern();

extern std::map<char, unsigned> BinopPrecedence;

#endif // MLIR_TOY_SRC_PARSER_H
