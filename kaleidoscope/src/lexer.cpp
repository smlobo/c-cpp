//
// Created by Sheldon Lobo on 2/14/26.
//

#include <fstream>
#include <string>
#include <iostream>
#include <map>
#include <memory>

#include "lexer.h"
#include "ast.h"

extern std::ifstream inputFile;

std::string IdentifierStr; // Filled in if tok_identifier
double NumVal; // Filled in if tok_number

/// gettok - Return the next token from standard input.
int gettok() {
    static int LastChar = ' ';

    // Skip any whitespace.
    while (isspace(LastChar))
        LastChar = inputFile.get();

    if (isalpha(LastChar)) {
        // identifier: [a-zA-Z][a-zA-Z0-9]*
        IdentifierStr = LastChar;
        while (isalnum(LastChar = inputFile.get()))
            IdentifierStr += LastChar;

        if (IdentifierStr == "def")
            return tok_def;
        if (IdentifierStr == "extern")
            return tok_extern;
        if (IdentifierStr == "if")
            return tok_if;
        if (IdentifierStr == "then")
            return tok_then;
        if (IdentifierStr == "else")
            return tok_else;
        return tok_identifier;
    }

    if (isdigit(LastChar) || LastChar == '.') {
        // Number: [0-9.]+
        std::string NumStr;
        do {
            NumStr += LastChar;
            LastChar = inputFile.get();
        } while (isdigit(LastChar) || LastChar == '.');

        NumVal = strtod(NumStr.c_str(), nullptr);
        return tok_number;
    }

    if (LastChar == '#') {
        // Comment until end of line.
        do
            LastChar = inputFile.get();
        while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

        if (LastChar != EOF)
            return gettok();
    }

    // Check for end of file.  Don't eat the EOF.
    if (LastChar == EOF)
        return tok_eof;

    // Otherwise, just return the character as its ascii value.
    int ThisChar = LastChar;
    LastChar = inputFile.get();
    return ThisChar;
}
