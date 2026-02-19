//
// Created by Sheldon Lobo on 2/15/26.
//

#ifndef KALEIDOSCOPE_PARSER_H
#define KALEIDOSCOPE_PARSER_H

extern int CurToken;
extern int getNextToken();

extern std::unique_ptr<FunctionAST> HandleDefinition();
extern void HandleExtern();
extern std::unique_ptr<FunctionAST> HandleExpression(std::unique_ptr<FunctionAST> FnAST);

#endif //KALEIDOSCOPE_PARSER_H