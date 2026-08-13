//
// Created by Sheldon Lobo on 2/15/26.
//

#ifndef KALEIDOSCOPE_PARSER_H
#define KALEIDOSCOPE_PARSER_H

#include <map>

extern int CurToken;
extern int getNextToken();

extern std::unique_ptr<FunctionAST> HandleDefinition();
extern void HandleExtern();

extern std::map<char, unsigned> BinopPrecedence;

#endif //KALEIDOSCOPE_PARSER_H