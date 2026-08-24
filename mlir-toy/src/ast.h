//
// Created by Sheldon Lobo on 8/10/26.
//

#ifndef MLIR_TOY_SRC_AST_H
#define MLIR_TOY_SRC_AST_H

#include <memory>
#include <string>
#include <vector>

#include "llvm/IR/Value.h"

//===----------------------------------------------------------------------===//
// Abstract Syntax Tree (aka Parse Tree)
//===----------------------------------------------------------------------===//

/// ExprAST - Base class for all expression nodes.
class ExprAST {
public:
    enum ExprASTKind {
        Expr_VarDecl,
        Expr_Return,
        Expr_Num,
        Expr_Literal,
        Expr_Var,
        Expr_UnaryOp,
        Expr_BinaryOp,
        Expr_Call,
        Expr_Print,
        Expr_If,
        Expr_For,
    };

private:
    ExprASTKind Kind;

public:
    ExprAST(ExprASTKind Kind);

    virtual ~ExprAST() = default;

    ExprASTKind getKind() const;

    virtual void print(int indent) = 0;
};

/// NumberExprAST - Expression class for numeric literals like "1.0".
class NumberExprAST : public ExprAST {
    double Val;

public:
    NumberExprAST(double Val);

    void print(int indent) override;
};

/// VariableExprAST - Expression class for referencing a variable, like "a".
class VariableExprAST : public ExprAST {
    std::string Name;
    std::vector<unsigned> Shape;

public:
    VariableExprAST(const std::string &Name);
    VariableExprAST(const std::string &Name, std::vector<unsigned> Shape);

    std::string getName() const;

    void print(int indent) override;
};

/// UnaryExprAST - Expression class for a unary operator.
class UnaryExprAST : public ExprAST {
    char Op;
    std::unique_ptr<ExprAST> Operand;

public:
    UnaryExprAST(char Op, std::unique_ptr<ExprAST> Operand);

    void print(int indent) override;
};

/// BinaryExprAST - Expression class for a binary operator.
class BinaryExprAST : public ExprAST {
    char Op;
    std::unique_ptr<ExprAST> LHS, RHS;

public:
    BinaryExprAST(char Op, std::unique_ptr<ExprAST> LHS, std::unique_ptr<ExprAST> RHS);

    char getOp() const;
    ExprAST* getLHS() const;
    ExprAST* getRHS() const;

    void print(int indent) override;
};

/// CallExprAST - Expression class for function calls.
class CallExprAST : public ExprAST {
    std::string Callee;
    std::vector<std::unique_ptr<ExprAST>> Args;

public:
    CallExprAST(const std::string &Callee, std::vector<std::unique_ptr<ExprAST> > Args);

    std::string getCallee() const;
    const std::vector<std::unique_ptr<ExprAST>>* getArgs() const;

    void print(int indent) override;
};

/// PrototypeAST - This class represents the "prototype" for a function, which captures its name,
/// and its argument names (thus implicitly the number of arguments the function takes).
/// As well as if it is an operator
class PrototypeAST {
    std::string Name;
    std::vector<std::string> Args;
    char Operator;
    unsigned Precedence;

public:
    PrototypeAST(const std::string &Name, std::vector<std::string> Args, char Operator,
                 unsigned Precedence);

    const std::string &getName() const;
    const std::vector<std::string> *getArgs() const;
    const char getOperator() const;
    bool isUnaryOp() const;
    bool isBinaryOp() const;
    unsigned getBinaryPrecedence() const;

    void print(int indent);
};

/// FunctionAST - This class represents a function definition itself.
class FunctionAST {
    std::unique_ptr<PrototypeAST> Proto;
    std::vector<std::unique_ptr<ExprAST>> BodyExprs;

public:
    FunctionAST(std::unique_ptr<PrototypeAST> Proto,
                std::vector<std::unique_ptr<ExprAST>> BodyExprs);

    const PrototypeAST *getProto() const;
    const std::vector<std::unique_ptr<ExprAST>> *getBody() const;

    void print(int indent);
};

/// IfExprAST - Expression class for if/then/else.
class IfExprAST : public ExprAST {
    std::unique_ptr<ExprAST> Cond, Then, Else;

public:
    IfExprAST(std::unique_ptr<ExprAST> Cond, std::unique_ptr<ExprAST> Then,
              std::unique_ptr<ExprAST> Else);

    void print(int indent) override;
};

/// ForExprAST - Expression class for for/in.
class ForExprAST : public ExprAST {
    std::string VarName;
    std::unique_ptr<ExprAST> Start, End, Stride, Body;

public:
    ForExprAST(std::string &VarName, std::unique_ptr<ExprAST> Start, std::unique_ptr<ExprAST> End,
               std::unique_ptr<ExprAST> Stride, std::unique_ptr<ExprAST> Body);

    void print(int indent) override;
};

/// VarExprAST - Expression class for var/in
class VarDeclExprAST : public ExprAST {
    std::unique_ptr<ExprAST> Var;

public:
    VarDeclExprAST(std::unique_ptr<ExprAST> Var);

    void print(int indent) override;
};

/// ReturnExprAST - Expression class for return
class ReturnExprAST : public ExprAST {
    std::unique_ptr<ExprAST> Return;

public:
    ReturnExprAST(std::unique_ptr<ExprAST> Return);

    ExprAST* getExpr() const;

    void print(int indent) override;
};

/// LiteralExprAST - Vector literals
class LiteralExprAST : public ExprAST {
    std::vector<std::unique_ptr<ExprAST>> Elements;

public:
    LiteralExprAST(std::vector<std::unique_ptr<ExprAST>> Elements);

    void print(int indent) override;
};

#endif // MLIR_TOY_SRC_AST_H
