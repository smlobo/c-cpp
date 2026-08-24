//
// Created by Sheldon Lobo on 8/10/26.
//

#ifndef MLIR_TOY_SRC_LEXER_H
#define MLIR_TOY_SRC_LEXER_H

#include <string>

// The lexer returns tokens [0-255] if it is an unknown character, otherwise one
// of these for known things.
enum Token {
    tok_eof = -1,

    // commands
    tok_def = -2,
    tok_extern = -3,

    // primary
    tok_identifier = -4,
    tok_number = -5,

    // conditional
    tok_if = -6,
    tok_then = -7,
    tok_else = -8,

    // loop
    tok_for = -9,
    tok_in = -10,

    // operators
    tok_binary = -11,
    tok_unary = -12,

    // variable declaration
    tok_var = -13,

    // return
    tok_return = -14,
};

extern std::string IdentifierStr; // Filled in if tok_identifier
extern double NumVal; // Filled in if tok_number
int gettok();

#endif // MLIR_TOY_SRC_LEXER_H
