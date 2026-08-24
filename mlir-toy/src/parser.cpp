//
// Created by Sheldon Lobo on 8/10/26.
//

#include "parser.h"

#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "ast.h"
#include "errors.h"
#include "lexer.h"
#include "main.h"

//===----------------------------------------------------------------------===//
// Parser
//===----------------------------------------------------------------------===//

/// CurTok/getNextToken - Provide a simple token buffer.  CurTok is the current
/// token the parser is looking at.  getNextToken reads another token from the
/// lexer and updates CurTok with its results.
int CurToken;

int getNextToken() {
    return CurToken = gettok();
}

/// BinopPrecedence - This holds the precedence for each binary operator that is
/// defined.
std::map<char, unsigned> BinopPrecedence = {
    {'=', 2},
    {'<', 10},
    {'+', 20},
    {'-', 20},
    {'*', 40},
};

/// GetTokPrecedence - Get the precedence of the pending binary operator token.
static int GetTokPrecedence() {
    if (!isascii(CurToken))
        return -1;

    // Make sure it's a declared binop.
    int TokPrec = BinopPrecedence[CurToken];
    if (TokPrec <= 0)
        return -1;
    return TokPrec;
}

static std::unique_ptr<ExprAST> ParseExpression();

/// numberexpr ::= number
static std::unique_ptr<ExprAST> ParseNumberExpr() {
    auto Result = std::make_unique<NumberExprAST>(NumVal);
    getNextToken(); // consume the number
    return std::move(Result);
}

/// parenexpr ::= '(' expression ')'
static std::unique_ptr<ExprAST> ParseParenExpr() {
    getNextToken(); // eat (.
    auto V = ParseExpression();
    if (!V)
        return nullptr;

    if (CurToken != ')')
        return LogError("expected ')'");
    getNextToken(); // eat ).
    return V;
}

/// identifierexpr
///   ::= identifier
///   ::= identifier '(' expression* ')'
static std::unique_ptr<ExprAST> ParseIdentifierExpr() {
    std::string IdName = IdentifierStr;

    getNextToken(); // eat identifier.

    // Simple variable ref.
    if (CurToken != '(') {
        // no vector shape
        if (CurToken != '<') {
            return std::make_unique<VariableExprAST>(IdName);
        }

        std::vector<unsigned> Shape;
        getNextToken(); // eat '<'
        while (CurToken != '>') {
            if (CurToken != tok_number)
                return LogError("expected number in <> shape");
            Shape.push_back(static_cast<unsigned>(NumVal));

            if (getNextToken() == ',')
                getNextToken(); // eat ','
        }
        getNextToken(); // eat '>'
        return std::make_unique<VariableExprAST>(IdName, Shape);
    }

    // Call.
    getNextToken(); // eat (
    std::vector<std::unique_ptr<ExprAST> > Args;
    if (CurToken != ')') {
        while (true) {
            if (auto Arg = ParseExpression())
                Args.push_back(std::move(Arg));
            else
                return nullptr;

            if (CurToken == ')')
                break;

            if (CurToken != ',')
                return LogError("Expected ')' or ',' in argument list");
            getNextToken();
        }
    }

    // Eat the ')'.
    getNextToken();

    return std::make_unique<CallExprAST>(IdName, std::move(Args));
}

/// ifexpr ::= 'if' expression 'then' expression 'else' expression
static std::unique_ptr<ExprAST> ParseIfExpr() {
    // eat if
    getNextToken();

    auto Cond = ParseExpression();
    if (!Cond)
        return nullptr;

    if (CurToken != tok_then)
        return LogError("expected then");
    // eat then
    getNextToken();

    auto Then = ParseExpression();
    if (!Then)
        return nullptr;

    if (CurToken != tok_else)
        return LogError("expected else");
    // eat else
    getNextToken();

    auto Else = ParseExpression();
    if (!Else)
        return nullptr;

    return std::make_unique<IfExprAST>(std::move(Cond), std::move(Then),
        std::move(Else));
}

/// forexpr ::= 'for' <start-expression>, <end-expression>, [stride-expression] 'in' expression
static std::unique_ptr<ExprAST> ParseForExpr() {
    // eat for
    getNextToken();

    if (CurToken != tok_identifier)
        return LogError("expected identifier after for");

    std::string IdName = IdentifierStr;
    // eat identifier.
    getNextToken();

    if (CurToken != '=')
        return LogError("expected '=' after for");
    // eat '='.
    getNextToken();

    auto Start = ParseExpression();
    if (!Start)
        return nullptr;

    if (CurToken != ',')
        return LogError("expected , between loop start and iteration end");
    // eat ,
    getNextToken();

    auto End = ParseExpression();
    if (!End)
        return nullptr;

    std::unique_ptr<ExprAST> Stride;
    if (CurToken == ',') {
        // eat ,
        getNextToken();

        Stride = ParseExpression();
        if (!Stride)
            return nullptr;
    } else {
        Stride = std::make_unique<NumberExprAST>(1.0);
    }

    if (CurToken != tok_in)
        return LogError("expected 'in' when parsing 'for'");
    // eat in
    getNextToken();

    auto Body = ParseExpression();
    if (!Body)
        return nullptr;

    return std::make_unique<ForExprAST>(IdName, std::move(Start), std::move(End),
        std::move(Stride), std::move(Body));
}

/// varexpr ::= 'var' expression
static std::unique_ptr<ExprAST> ParseVarExpr() {
    // eat var
    getNextToken();

    if (CurToken != tok_identifier)
        return LogError("expected identifier after var");

    auto expr = ParseExpression();
    if (!expr)
        return nullptr;

    return std::make_unique<VarDeclExprAST>(std::move(expr));
}

/// returnexpr ::= 'return' expression
static std::unique_ptr<ExprAST> ParseReturnExpr() {
    // eat return
    getNextToken();

    auto returnExpr = ParseExpression();
    if (!returnExpr)
        return nullptr;

    return std::make_unique<ReturnExprAST>(std::move(returnExpr));
}

/// literalexpr :: '[' <expr1>, [exprn] ']'
static std::unique_ptr<ExprAST> ParseLiteralExpr() {
    getNextToken(); // eat '['

    std::vector<std::unique_ptr<ExprAST>> LiteralExprs;
    while (CurToken != ']') {
        auto Expr = ParseExpression();
        LiteralExprs.push_back(std::move(Expr));

        if (CurToken == ',')
            getNextToken(); // eat ','
    }

    getNextToken(); // eat ']'

    return std::make_unique<LiteralExprAST>(std::move(LiteralExprs));
}

/// primary
///   ::= identifierexpr
///   ::= numberexpr
///   ::= parenexpr
///   ::= ifexpr
///   ::= forexpr
///   ::= varexpr
///   ::= returnexpr
static std::unique_ptr<ExprAST> ParsePrimary() {
    switch (CurToken) {
        default:
            return LogError("unknown token when expecting an expression");
        case tok_identifier:
            return ParseIdentifierExpr();
        case tok_number:
            return ParseNumberExpr();
        case '(':
            return ParseParenExpr();
        case tok_if:
            return ParseIfExpr();
        case tok_for:
            return ParseForExpr();
        case tok_var:
            return ParseVarExpr();
        case tok_return:
            return ParseReturnExpr();
        case '[':
            return ParseLiteralExpr();
    }
}

/// unary
///   ::= primary
///   ::= '!' unary
static std::unique_ptr<ExprAST> ParseUnary() {
    // If the current token is not an operator, it must be a primary expr.
    if (!isascii(CurToken) || CurToken == '(' || CurToken == ',' || CurToken == '[') {}
        return ParsePrimary();

    // If this is a unary operator, read it.
    int Opc = CurToken;
    getNextToken();
    if (auto Operand = ParseUnary())
        return std::make_unique<UnaryExprAST>(Opc, std::move(Operand));
    return nullptr;
}

/// binoprhs
///   ::= ('+' primary)*
static std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec,
                                              std::unique_ptr<ExprAST> LHS) {
    // If this is a binop, find its precedence.
    while (true) {
        int TokPrec = GetTokPrecedence();

        // If this is a binop that binds at least as tightly as the current binop,
        // consume it, otherwise we are done.
        if (TokPrec < ExprPrec)
            return LHS;

        // Okay, we know this is a binop.
        int BinOp = CurToken;
        getNextToken(); // eat binop

        // Parse the unary expression after the binary operator.
        auto RHS = ParseUnary();
        if (!RHS)
            return nullptr;

        // If BinOp binds less tightly with RHS than the operator after RHS, let
        // the pending operator take RHS as its LHS.
        int NextPrec = GetTokPrecedence();
        if (TokPrec < NextPrec) {
            RHS = ParseBinOpRHS(TokPrec + 1, std::move(RHS));
            if (!RHS)
                return nullptr;
        }

        // Merge LHS/RHS.
        LHS = std::make_unique<BinaryExprAST>(BinOp, std::move(LHS), std::move(RHS));
    }
}

/// expression
///   ::= unary binoprhs
///
static std::unique_ptr<ExprAST> ParseExpression() {
    auto LHS = ParseUnary();
    if (!LHS)
        return nullptr;

    return ParseBinOpRHS(0, std::move(LHS));
}

/// prototype
///   ::= id '(' id* ')'
///   ::= binary LETTER number? (id, id)
static std::unique_ptr<PrototypeAST> ParsePrototype() {
    std::string FnName;
    unsigned operatorKind = 0;
    unsigned BinaryPrecedence = 30;
    char Operator = '\0';

    switch (CurToken) {
        case tok_identifier:
            FnName = IdentifierStr;
            getNextToken();
            break;
        case tok_unary:
            getNextToken();
            if (!isascii(CurToken))
                return LogErrorP("Expected unary operator");
            FnName = "unary";
            Operator = static_cast<char>(CurToken);
            FnName += Operator;
            getNextToken();
            operatorKind = 1;
            break;
        case tok_binary:
            getNextToken();
            if (!isascii(CurToken))
                return LogErrorP("Expected binary operator");
            FnName = "binary";
            Operator = static_cast<char>(CurToken);
            FnName += Operator;
            getNextToken();
            // Read the precedence if present
            if (CurToken == tok_number) {
                if (NumVal < 1 || NumVal > 100)
                    return LogErrorP("Expected number after unary operator");
                BinaryPrecedence = static_cast<unsigned>(NumVal);
                getNextToken();
            }
            operatorKind = 2;
            break;
        default:
            return LogErrorP("Expected function name in prototype");
    }

    if (CurToken != '(')
        return LogErrorP(std::string("Expected '(' in prototype, got: " + std::to_string(CurToken)).c_str());

    std::vector<std::string> ArgNames;
    while (getNextToken() == tok_identifier) {
        ArgNames.push_back(IdentifierStr);
        if (getNextToken() != ',')
            break;
    }
    if (CurToken != ')')
        return LogErrorP("Expected ')' in prototype");

    if (operatorKind && ArgNames.size() != operatorKind)
        return LogErrorP("Invalid number of operands for operator");

    return std::make_unique<PrototypeAST>(FnName, std::move(ArgNames), Operator,
        BinaryPrecedence);
}

/// definition ::= 'def' prototype expression
static std::unique_ptr<FunctionAST> ParseDefinition() {
    // eat def
    getNextToken();

    auto Proto = ParsePrototype();
    if (!Proto)
        return nullptr;

    if (getNextToken() != '{')
        return nullptr;

    std::vector<std::unique_ptr<ExprAST>> BodyExprs;
    while (getNextToken() != '}') {
        auto Expr = ParseExpression();
        BodyExprs.push_back(std::move(Expr));
    }

    getNextToken(); // eat '}'

    return std::make_unique<FunctionAST>(std::move(Proto), std::move(BodyExprs));
}

/// external ::= 'extern' prototype
static std::unique_ptr<PrototypeAST> ParseExtern() {
    getNextToken(); // eat extern.
    return ParsePrototype();
}

//===----------------------------------------------------------------------===//
// Top-Level parsing
//===----------------------------------------------------------------------===//

std::unique_ptr<FunctionAST> HandleDefinition() {
    if (auto FnAST = ParseDefinition()) {
        if (ast_dump) {
            FnAST->print(0);
        }
        return FnAST;
    }

    // Skip token for error recovery.
    getNextToken();
    return nullptr;
}

void HandleExtern() {
    if (auto ProtoAST = ParseExtern()) {
        if (ast_dump) {
            ProtoAST->print(0);
        }
    } else {
        // Skip token for error recovery.
        getNextToken();
    }
}
