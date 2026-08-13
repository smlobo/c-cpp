//
// Created by Sheldon Lobo on 2/14/26.
//

#include <string>
#include <vector>
#include <iostream>

#include <llvm/Support/raw_ostream.h>

#include "ast.h"
#include "main.h"

//===----------------------------------------------------------------------===//
// Abstract Syntax Tree (aka Parse Tree)
//===----------------------------------------------------------------------===//

NumberExprAST::NumberExprAST(double Val) : Val(Val) {}

void NumberExprAST::print(int indent) {
    llvm::errs().indent(indent);
    llvm::errs().changeColor(llvm::raw_ostream::GREEN, true);
    llvm::errs() << "NumberExprAST: ";
    llvm::errs().changeColor(llvm::raw_ostream::BRIGHT_GREEN, true);
    llvm::errs() << std::format("{:.2f}", Val) << "\n";
    llvm::errs().resetColor();
}

VariableExprAST::VariableExprAST(const std::string &Name)
    : Name(Name), Shape(std::vector<unsigned>()) {}

VariableExprAST::VariableExprAST(const std::string &Name, std::vector<unsigned> Shape)
    : Name(Name), Shape(std::move(Shape)) {}

void VariableExprAST::print(int indent) {
    llvm::errs().indent(indent);
    llvm::errs().changeColor(llvm::raw_ostream::BLUE, true);
    llvm::errs() << "VariableExprAST: ";
    llvm::errs().changeColor(llvm::raw_ostream::BRIGHT_BLUE, true);
    llvm::errs() << Name << "\n";
    llvm::errs().resetColor();
}

UnaryExprAST::UnaryExprAST(char Op, std::unique_ptr<ExprAST> Operand)
    : Op(Op), Operand(std::move(Operand)) {}

void UnaryExprAST::print(int indent) {
    llvm::errs().indent(indent);
    llvm::errs().changeColor(llvm::raw_ostream::MAGENTA, true);
    llvm::errs() << "UnaryExprAST: ";
    llvm::errs().changeColor(llvm::raw_ostream::BRIGHT_MAGENTA, true);
    llvm::errs() << Op << "\n";
    llvm::errs().resetColor();
    Operand->print(indent + 2);
}

BinaryExprAST::BinaryExprAST(char Op, std::unique_ptr<ExprAST> LHS, std::unique_ptr<ExprAST> RHS)
    : Op(Op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}

void BinaryExprAST::print(int indent) {
    llvm::errs().indent(indent);
    llvm::errs().changeColor(llvm::raw_ostream::MAGENTA, true);
    llvm::errs() << "BinaryExprAST: ";
    llvm::errs().changeColor(llvm::raw_ostream::BRIGHT_MAGENTA, true);
    llvm::errs() << Op << "\n";
    llvm::errs().resetColor();
    LHS->print(indent + 2);
    RHS->print(indent + 2);
}

CallExprAST::CallExprAST(const std::string &Callee,
                         std::vector<std::unique_ptr<ExprAST>> Args)
    : Callee(Callee), Args(std::move(Args)) {}

void CallExprAST::print(int indent) {
    llvm::errs().indent(indent);
    llvm::errs().changeColor(llvm::raw_ostream::CYAN, true);
    llvm::errs() << "CallExprAST: ";
    llvm::errs().changeColor(llvm::raw_ostream::BRIGHT_CYAN, true);
    llvm::errs() << Callee << "\n";
    llvm::errs().resetColor();
    for (const auto &Arg : Args) {
        Arg->print(indent + 2);
    }
}

PrototypeAST::PrototypeAST(const std::string &Name, std::vector<std::string> Args, char Operator,
                           unsigned Precedence)
    : Name(Name), Args(std::move(Args)), Operator(Operator), Precedence(Precedence) {}

const std::string &PrototypeAST::getName() const {
    return Name;
}

const char PrototypeAST::getOperator() const {
    return Operator;
}

bool PrototypeAST::isUnaryOp() const {
    return Operator != '\0' && Args.size() == 1;
}

bool PrototypeAST::isBinaryOp() const {
    return Operator != '\0' && Args.size() == 2;
}

unsigned PrototypeAST::getBinaryPrecedence() const {
    return Precedence;
}

void PrototypeAST::print(int indent) {
    llvm::errs().indent(indent);
    llvm::errs().changeColor(llvm::raw_ostream::CYAN, true);
    llvm::errs() << "PrototypeAST: ";
    llvm::errs().changeColor(llvm::raw_ostream::BRIGHT_CYAN, true);
    llvm::errs() << Name << " (";
    for (const auto &Arg : Args) {
        llvm::errs() << Arg << ", ";
    }
    llvm::errs() << ")\n";
    llvm::errs().resetColor();
}

FunctionAST::FunctionAST(std::unique_ptr<PrototypeAST> Proto,
                         std::vector<std::unique_ptr<ExprAST>> BodyExprs)
    : Proto(std::move(Proto)), BodyExprs(std::move(BodyExprs)) {}

void FunctionAST::print(int indent) {
    llvm::errs().indent(indent);
    llvm::errs().changeColor(llvm::raw_ostream::YELLOW, true);
    llvm::errs() << "FunctionAST:\n";
    llvm::errs().resetColor();
    Proto->print(indent + 2);
    for (const auto &BodyExpr : BodyExprs) {
        BodyExpr->print(indent + 2);
    }
}

IfExprAST::IfExprAST(std::unique_ptr<ExprAST> Cond, std::unique_ptr<ExprAST> Then,
                     std::unique_ptr<ExprAST> Else)
    : Cond(std::move(Cond)), Then(std::move(Then)), Else(std::move(Else)) {}

void IfExprAST::print(int indent) {
    llvm::errs().indent(indent);
    llvm::errs().changeColor(llvm::raw_ostream::RED, true);
    llvm::errs() << "IfExprAST:\n";
    llvm::errs().resetColor();
    Cond->print(indent + 2);
    Then->print(indent + 2);
    Else->print(indent + 2);
}

ForExprAST::ForExprAST(std::string &VarName, std::unique_ptr<ExprAST> Start,
                       std::unique_ptr<ExprAST> End, std::unique_ptr<ExprAST> Stride,
                       std::unique_ptr<ExprAST> Body)
    : VarName(VarName), Start(std::move(Start)), End(std::move(End)),
      Stride(std::move(Stride)), Body(std::move(Body)) {}

void ForExprAST::print(int indent) {
    llvm::errs().indent(indent);
    llvm::errs().changeColor(llvm::raw_ostream::RED, true);
    llvm::errs() << "ForExprAST: ";
    llvm::errs().changeColor(llvm::raw_ostream::BRIGHT_RED, true);
    llvm::errs() << VarName << "\n";
    llvm::errs().resetColor();
    Start->print(indent + 2);
    End->print(indent + 2);
    Stride->print(indent + 2);
    Body->print(indent + 2);
}

VarExprAST::VarExprAST(std::unique_ptr<ExprAST> Var)
    : Var(std::move(Var)) {}

void VarExprAST::print(int indent) {
    llvm::errs().indent(indent);
    llvm::errs().changeColor(llvm::raw_ostream::RED, true);
    llvm::errs() << "VarExprAST: ";
    llvm::errs().resetColor();
    llvm::errs() << "\n";
    Var->print(indent + 2);
}

ReturnExprAST::ReturnExprAST(std::unique_ptr<ExprAST> Return)
    : Return(std::move(Return)) {}

void ReturnExprAST::print(int indent) {
    llvm::errs().indent(indent);
    llvm::errs().changeColor(llvm::raw_ostream::RED, true);
    llvm::errs() << "ReturnExprAST:\n";
    llvm::errs().resetColor();
    Return->print(indent + 2);
}

LiteralExprAST::LiteralExprAST(std::vector<std::unique_ptr<ExprAST>> Elements)
    : Elements(std::move(Elements)) {}

void LiteralExprAST::print(int indent) {
    llvm::errs().indent(indent);
    llvm::errs().changeColor(llvm::raw_ostream::CYAN, true);
    llvm::errs() << "LiteralExprAST: <" << std::to_string(Elements.size()) << ">[\n";
    llvm::errs().resetColor();
    for (const auto &Element : Elements)
        Element->print(indent + 2);
    llvm::errs().indent(indent);
    llvm::errs().changeColor(llvm::raw_ostream::CYAN, true);
    llvm::errs() << "]\n";
    llvm::errs().resetColor();
}