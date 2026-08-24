//
// Created by Sheldon Lobo on 8/16/26.
//

#ifndef MLIR_TOY_SRC_MLIRGEN_H
#define MLIR_TOY_SRC_MLIRGEN_H

#include <memory>
#include <vector>

#include "llvm/ADT/ScopedHashTable.h"
#include "mlir/IR/BuiltinOps.h"
#include "mlir/IR/MLIRContext.h"

#include "ast.h"
#include "Dialect.h"

class MLIRGenImpl {
private:
    /// A "module" matches a Toy source file: containing a list of functions.
    mlir::ModuleOp theModule;

    /// The builder is a helper class to create IR inside a function. The builder
    /// is stateful, in particular it keeps an "insertion point": this is where
    /// the next operations will be introduced.
    mlir::OpBuilder builder;

    /// The symbol table maps a variable name to a value in the current scope.
    /// Entering a function creates a new scope, and the function arguments are
    /// added to the mapping. When the processing of a function is terminated, the
    /// scope is destroyed and the mappings created in this scope are dropped.
    llvm::ScopedHashTable<llvm::StringRef, mlir::Value> symbolTable;

    /// Unknown location helper
    mlir::Location loc;

    /// Declare a variable in the current scope, returning failure if it was
    /// already declared there.
    llvm::LogicalResult declare(llvm::StringRef var, mlir::Value value);

    /// Emit a new function and add it to the MLIR module.
    mlir::toy::FuncOp mlirGen(const FunctionAST &funcAST);

    /// Create the prototype for an MLIR function with as many arguments as the
    /// provided Toy AST prototype.
    mlir::toy::FuncOp mlirGen(const PrototypeAST &proto);

    /// Codegen a list of expression, return failure if one of them hit an error.
    llvm::LogicalResult mlirGen(const std::vector<std::unique_ptr<ExprAST>> &blockAST);

    /// Handle a variable declaration, we'll codegen the expression that forms the
    /// initializer and record the value in the symbol table before returning it.
    /// Future expressions will be able to reference this variable through symbol
    /// table lookup.
    // mlir::Value mlirGen(VarDeclExprAST &vardecl);

    /// Emit a return operation. This will return failure if any generation fails.
    llvm::LogicalResult mlirGen(ReturnExprAST &ret);

    /// Dispatch codegen for the right expression subclass using RTTI.
    mlir::Value mlirGen(ExprAST &expr);

    /// Emit a binary operation
    mlir::Value mlirGen(BinaryExprAST &binop);

    /// Emit a call expression. It emits specific operations for the `transpose`
    /// builtin. Other identifiers are assumed to be user-defined functions.
    mlir::Value mlirGen(CallExprAST &call);

    /// This is a reference to a variable in an expression. The variable is
    /// expected to have been declared and so should have a value in the symbol
    /// table, otherwise emit an error and return nullptr.
    mlir::Value mlirGen(VariableExprAST &expr);

public:
    MLIRGenImpl(mlir::MLIRContext &context);

    /// Public API: convert the AST for a Toy module (source file) to an MLIR
    /// Module operation.
    mlir::ModuleOp mlirGen(std::vector<std::unique_ptr<FunctionAST>> &functions);
};

#endif // MLIR_TOY_SRC_MLIRGEN_H
