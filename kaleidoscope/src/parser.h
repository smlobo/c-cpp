//
// Created by Sheldon Lobo on 2/15/26.
//

#ifndef KALEIDOSCOPE_PARSER_H
#define KALEIDOSCOPE_PARSER_H

extern int CurToken;
extern int getNextToken();

extern void HandleDefinition();
extern void HandleExtern();
extern void HandleTopLevelExpression();

#endif //KALEIDOSCOPE_PARSER_H