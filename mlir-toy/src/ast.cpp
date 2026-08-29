//
// Created by Sheldon Lobo on 8/10/26.
//

#include "ast.h"

#include <format>
#include <utility>

#include "llvm/Support/raw_ostream.h"

//===----------------------------------------------------------------------===//
// Abstract Syntax Tree (aka Parse Tree)
//===----------------------------------------------------------------------===//

ExprAST::ExprAST(ExprASTKind Kind) : Kind(Kind) {}

ExprAST::ExprASTKind ExprAST::getKind() const {
    return Kind;
}

NumberExprAST::NumberExprAST(double Val) : ExprAST(Expr_Num), Val(Val) {}

void NumberExprAST::print(int indent) {
    llvm::errs().indent(indent);
    llvm::errs().changeColor(llvm::raw_ostream::GREEN, true);
    llvm::errs() << "NumberExprAST: ";
    llvm::errs().changeColor(llvm::raw_ostream::BRIGHT_GREEN, true);
    llvm::errs() << std::format("{:.2f}", Val);
    llvm::errs().resetColor();
    llvm::errs() << "\n";
}

VariableExprAST::VariableExprAST(const std::string &Name)
    : ExprAST(Expr_Var), Name(Name), Shape(std::vector<unsigned>()) {}

VariableExprAST::VariableExprAST(const std::string &Name,
        std::vector<unsigned> Shape)
    : ExprAST(Expr_Var), Name(Name), Shape(std::move(Shape)) {}

std::string VariableExprAST::getName() const {
    return Name;
}

void VariableExprAST::print(int indent) {
    llvm::errs().indent(indent);
    llvm::errs().changeColor(llvm::raw_ostream::BLUE, true);
    llvm::errs() << "VariableExprAST: ";
    llvm::errs().changeColor(llvm::raw_ostream::BRIGHT_BLUE, true);
    llvm::errs() << Name;
    llvm::errs().resetColor();
    llvm::errs() << "\n";
}

UnaryExprAST::UnaryExprAST(char Op, std::unique_ptr<ExprAST> Operand)
    : ExprAST(Expr_UnaryOp), Op(Op), Operand(std::move(Operand)) {}

void UnaryExprAST::print(int indent) {
    llvm::errs().indent(indent);
    llvm::errs().changeColor(llvm::raw_ostream::MAGENTA, true);
    llvm::errs() << "UnaryExprAST: ";
    llvm::errs().changeColor(llvm::raw_ostream::BRIGHT_MAGENTA, true);
    llvm::errs() << Op;
    llvm::errs().resetColor();
    llvm::errs() << "\n";
    Operand->print(indent + 2);
}

BinaryExprAST::BinaryExprAST(char Op, std::unique_ptr<ExprAST> LHS,
        std::unique_ptr<ExprAST> RHS)
    : ExprAST(Expr_BinaryOp), Op(Op), LHS(std::move(LHS)),
        RHS(std::move(RHS)) {}

char BinaryExprAST::getOp() const {
    return Op;
}

ExprAST *BinaryExprAST::getLHS() const {
    return LHS.get();
}

ExprAST *BinaryExprAST::getRHS() const {
    return RHS.get();
}

void BinaryExprAST::print(int indent) {
    llvm::errs().indent(indent);
    llvm::errs().changeColor(llvm::raw_ostream::MAGENTA, true);
    llvm::errs() << "BinaryExprAST: ";
    llvm::errs().changeColor(llvm::raw_ostream::BRIGHT_MAGENTA, true);
    llvm::errs() << Op;
    llvm::errs().resetColor();
    llvm::errs() << "\n";
    LHS->print(indent + 2);
    RHS->print(indent + 2);
}

CallExprAST::CallExprAST(const std::string &Callee,
        std::vector<std::unique_ptr<ExprAST>> Args)
    : ExprAST(Expr_Call), Callee(Callee), Args(std::move(Args)) {}

std::string CallExprAST::getCallee() const {
    return Callee;
}

const std::vector<std::unique_ptr<ExprAST>>* CallExprAST::getArgs() const {
    return &Args;
}

void CallExprAST::print(int indent) {
    llvm::errs().indent(indent);
    llvm::errs().changeColor(llvm::raw_ostream::CYAN, true);
    llvm::errs() << "CallExprAST: ";
    llvm::errs().changeColor(llvm::raw_ostream::BRIGHT_CYAN, true);
    llvm::errs() << Callee;
    llvm::errs().resetColor();
    llvm::errs() << "\n";
    for (const auto &Arg : Args) {
        Arg->print(indent + 2);
    }
}

PrototypeAST::PrototypeAST(const std::string &Name, std::vector<std::string> Args,
        char Operator, unsigned Precedence)
    : Name(Name), Args(std::move(Args)), Operator(Operator),
        Precedence(Precedence) {}

const std::string &PrototypeAST::getName() const {
    return Name;
}

const std::vector<std::string> *PrototypeAST::getArgs() const {
    return &Args;
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
    llvm::errs() << ")";
    llvm::errs().resetColor();
    llvm::errs() << "\n";
}

FunctionAST::FunctionAST(std::unique_ptr<PrototypeAST> Proto,
        std::vector<std::unique_ptr<ExprAST>> BodyExprs)
    : Proto(std::move(Proto)), BodyExprs(std::move(BodyExprs)) {}

const PrototypeAST *FunctionAST::getProto() const {
    return Proto.get();
}

const std::vector<std::unique_ptr<ExprAST>> *FunctionAST::getBody() const {
    return &BodyExprs;
}

void FunctionAST::print(int indent) {
    llvm::errs().indent(indent);
    llvm::errs().changeColor(llvm::raw_ostream::YELLOW, true);
    llvm::errs() << "FunctionAST:";
    llvm::errs().resetColor();
    llvm::errs() << "\n";
    Proto->print(indent + 2);
    for (const auto &BodyExpr : BodyExprs) {
        BodyExpr->print(indent + 2);
    }
}

IfExprAST::IfExprAST(std::unique_ptr<ExprAST> Cond, std::unique_ptr<ExprAST> Then,
        std::unique_ptr<ExprAST> Else)
    : ExprAST(Expr_If), Cond(std::move(Cond)), Then(std::move(Then)),
        Else(std::move(Else)) {}

void IfExprAST::print(int indent) {
    llvm::errs().indent(indent);
    llvm::errs().changeColor(llvm::raw_ostream::RED, true);
    llvm::errs() << "IfExprAST:";
    llvm::errs().resetColor();
    llvm::errs() << "\n";
    Cond->print(indent + 2);
    Then->print(indent + 2);
    Else->print(indent + 2);
}

ForExprAST::ForExprAST(std::string &VarName, std::unique_ptr<ExprAST> Start,
        std::unique_ptr<ExprAST> End, std::unique_ptr<ExprAST> Stride,
        std::unique_ptr<ExprAST> Body)
    : ExprAST(Expr_For), VarName(VarName), Start(std::move(Start)),
        End(std::move(End)), Stride(std::move(Stride)),
        Body(std::move(Body)) {}

void ForExprAST::print(int indent) {
    llvm::errs().indent(indent);
    llvm::errs().changeColor(llvm::raw_ostream::RED, true);
    llvm::errs() << "ForExprAST: ";
    llvm::errs().changeColor(llvm::raw_ostream::BRIGHT_RED, true);
    llvm::errs() << VarName;
    llvm::errs().resetColor();
    llvm::errs() << "\n";
    Start->print(indent + 2);
    End->print(indent + 2);
    Stride->print(indent + 2);
    Body->print(indent + 2);
}

VarDeclExprAST::VarDeclExprAST(std::unique_ptr<ExprAST> Var)
    : ExprAST(Expr_VarDecl), Var(std::move(Var)) {}

void VarDeclExprAST::print(int indent) {
    llvm::errs().indent(indent);
    llvm::errs().changeColor(llvm::raw_ostream::RED, true);
    llvm::errs() << "VarExprAST: ";
    llvm::errs().resetColor();
    llvm::errs() << "\n";
    Var->print(indent + 2);
}

ReturnExprAST::ReturnExprAST(std::unique_ptr<ExprAST> Return)
    : ExprAST(Expr_Return), Return(std::move(Return)) {}

ExprAST* ReturnExprAST::getExpr() const {
    return Return.get();
}

void ReturnExprAST::print(int indent) {
    llvm::errs().indent(indent);
    llvm::errs().changeColor(llvm::raw_ostream::RED, true);
    llvm::errs() << "ReturnExprAST:";
    llvm::errs().resetColor();
    llvm::errs() << "\n";
    Return->print(indent + 2);
}

LiteralExprAST::LiteralExprAST(std::vector<std::unique_ptr<ExprAST>> Elements)
    : ExprAST(Expr_Literal), Elements(std::move(Elements)) {}

void LiteralExprAST::print(int indent) {
    llvm::errs().indent(indent);
    llvm::errs().changeColor(llvm::raw_ostream::CYAN, true);
    llvm::errs() << "LiteralExprAST: <" << std::to_string(Elements.size()) << ">[";
    llvm::errs().resetColor();
    llvm::errs() << "\n";
    for (const auto &Element : Elements)
        Element->print(indent + 2);
    llvm::errs().indent(indent);
    llvm::errs().changeColor(llvm::raw_ostream::CYAN, true);
    llvm::errs() << "]";
    llvm::errs().resetColor();
    llvm::errs() << "\n";
}
