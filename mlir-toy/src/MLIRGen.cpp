//
// Created by Sheldon Lobo on 8/16/26.
//

#include "MLIRGen.h"

#include <llvm/IR/Intrinsics.h>

#include "mlir/IR/Builders.h"
#include "mlir/IR/Verifier.h"

#include "Dialect.h"

MLIRGenImpl::MLIRGenImpl(mlir::MLIRContext &context)
    : builder(&context), loc(builder.getUnknownLoc()) {}

/// Declare a variable in the current scope, returning failure if it was
/// already declared there.
llvm::LogicalResult MLIRGenImpl::declare(llvm::StringRef var,
                                         mlir::Value value) {
    if (symbolTable.count(var))
        return mlir::failure();
    symbolTable.insert(var, value);
    return mlir::success();
}

/// Convert the AST for a Toy module (source file) to an MLIR
/// Module operation.
mlir::ModuleOp MLIRGenImpl::mlirGen(std::vector<std::unique_ptr<FunctionAST>> &functions) {
    // We create an empty MLIR module and codegen functions one at a time and
    // add them to the module.
    theModule = mlir::ModuleOp::create(builder.getUnknownLoc());

    for (const auto &f : functions)
        mlirGen(*f);

    // Verify the module after we have finished constructing it, this will check
    // the structural properties of the IR and invoke any specific verifiers we
    // have on the Toy operations.
    if (llvm::failed(mlir::verify(theModule))) {
        theModule.emitError("module verification error");
        return nullptr;
    }

    return theModule;
}

/// Emit a new function and add it to the MLIR module.
mlir::toy::FuncOp MLIRGenImpl::mlirGen(const FunctionAST &funcAST) {
    // Create a scope in the symbol table to hold variable declarations.
    llvm::ScopedHashTableScope<llvm::StringRef, mlir::Value> varScope(symbolTable);

    const PrototypeAST *proto = funcAST.getProto();

    // Create an MLIR function for the given prototype.
    builder.setInsertionPointToEnd(theModule.getBody());
    mlir::toy::FuncOp function = mlirGen(*proto);
    if (!function)
        return nullptr;

    // Let's start the body of the function now!
    mlir::Block &entryBlock = function.front();

    // Declare all the function arguments in the symbol table.
    for (const auto &arg : llvm::enumerate(entryBlock.getArguments())) {
        if (mlir::failed(declare((*proto->getArgs())[arg.index()], arg.value())))
            return nullptr;
    }

    // Set the insertion point in the builder to the beginning of the function
    // body, it will be used throughout the codegen to create operations in this
    // function.
    builder.setInsertionPointToStart(&entryBlock);

    // Emit the body of the function.
    if (mlir::failed(mlirGen(*funcAST.getBody()))) {
        function.erase();
        return nullptr;
    }

    return function;
}

/// Create the prototype for an MLIR function with as many arguments as the
/// provided Toy AST prototype.
mlir::toy::FuncOp MLIRGenImpl::mlirGen(const PrototypeAST &proto) {
    // This is a generic function, the return type will be inferred later.
    // Arguments type are uniformly unranked tensors.
    llvm::SmallVector<mlir::Type, 4> argTypes(proto.getArgs()->size(),
                                              mlir::UnrankedTensorType::get(builder.getF64Type()));
    auto funcType = builder.getFunctionType(argTypes, {});
    return mlir::toy::FuncOp::create(builder, loc, proto.getName(), funcType);
}

/// Codegen a list of expression, return failure if one of them hit an error.
llvm::LogicalResult MLIRGenImpl::mlirGen(const std::vector<std::unique_ptr<ExprAST>> &blockAST) {
    llvm::ScopedHashTableScope<llvm::StringRef, mlir::Value> varScope(symbolTable);

    for (const auto &bodyExpr: blockAST) {
        // Specific handling for variable declarations, return statement. These
        // can only appear in block list and not in nested expressions.
        // if (bodyExpr->getKind() == ExprAST::Expr_VarDecl) {
        //     if (!mlirGen(*static_cast<VarDeclExprAST*>(bodyExpr.get())))
        //         return mlir::failure();
        // }
        if (bodyExpr->getKind() == ExprAST::Expr_Return) {
            if (mlir::failed(mlirGen(*static_cast<ReturnExprAST*>(bodyExpr.get()))))
                return mlir::failure();
        }
    }
    return mlir::success();
}

/// Handle a variable declaration, we'll codegen the expression that forms the
/// initializer and record the value in the symbol table before returning it.
/// Future expressions will be able to reference this variable through symbol
/// table lookup.
// mlir::Value MLIRGenImpl::mlirGen(VarDeclExprAST &vardecl) {
//     auto *init = vardecl.getInitVal();
//     if (!init) {
//         emitError(loc(vardecl.loc()),
//                   "missing initializer in variable declaration");
//         return nullptr;
//     }
//
//     mlir::Value value = mlirGen(*init);
//     if (!value)
//         return nullptr;
//
//     // We have the initializer value, but in case the variable was declared
//     // with specific shape, we emit a "reshape" operation. It will get
//     // optimized out later as needed.
//     if (!vardecl.getType().shape.empty()) {
//         value = ReshapeOp::create(builder, loc(vardecl.loc()),
//                                   getType(vardecl.getType()), value);
//     }
//
//     // Register the value in the symbol table.
//     if (failed(declare(vardecl.getName(), value)))
//         return nullptr;
//     return value;
// }

/// Emit a return operation. This will return failure if any generation fails.
llvm::LogicalResult MLIRGenImpl::mlirGen(ReturnExprAST &ret) {
    // 'return' takes an optional expression, handle that case here.
    mlir::Value expr = nullptr;
    if (ret.getExpr()) {
        expr = mlirGen(*ret.getExpr());
        if (!expr)
            return mlir::failure();

        // Prototypes start with no result type; infer it from the value being
        // returned so that toy.return matches its enclosing function.
        auto function = mlir::cast<mlir::toy::FuncOp>(
            builder.getInsertionBlock()->getParentOp());
        function.setFunctionType(
            builder.getFunctionType(function.getArgumentTypes(), expr.getType()));
    }

    // Otherwise, this return operation has zero operands.
    mlir::toy::ReturnOp::create(builder, loc,
                     expr ? llvm::ArrayRef(expr) : llvm::ArrayRef<mlir::Value>());
    return mlir::success();
}

/// Dispatch codegen for the right expression subclass using RTTI.
mlir::Value MLIRGenImpl::mlirGen(ExprAST &expr) {
    switch (expr.getKind()) {
        case ExprAST::Expr_BinaryOp:
            return mlirGen(*static_cast<BinaryExprAST*>(&expr));
        case ExprAST::Expr_Var:
            return mlirGen(*static_cast<VariableExprAST*>(&expr));
        case ExprAST::Expr_Literal:
            return mlirGen(*static_cast<LiteralExprAST*>(&expr));
        case ExprAST::Expr_Call:
            return mlirGen(*static_cast<CallExprAST*>(&expr));
        case ExprAST::Expr_Num:
            return mlirGen(*static_cast<NumberExprAST*>(&expr));
        default:
            mlir::emitError(loc)
                << "MLIR codegen encountered an unhandled expr kind '"
                << llvm::Twine(expr.getKind()) << "'";
            return nullptr;
    }
}

/// Emit a binary operation
mlir::Value MLIRGenImpl::mlirGen(BinaryExprAST &binop) {
    // First emit the operations for each side of the operation before emitting
    // the operation itself. For example if the expression is `a + foo(a)`
    // 1) First it will visiting the LHS, which will return a reference to the
    //    value holding `a`. This value should have been emitted at declaration
    //    time and registered in the symbol table, so nothing would be
    //    codegen'd. If the value is not in the symbol table, an error has been
    //    emitted and nullptr is returned.
    // 2) Then the RHS is visited (recursively) and a call to `foo` is emitted
    //    and the result value is returned. If an error occurs we get a nullptr
    //    and propagate.
    //
    mlir::Value lhs = mlirGen(*binop.getLHS());
    if (!lhs)
        return nullptr;
    mlir::Value rhs = mlirGen(*binop.getRHS());
    if (!rhs)
        return nullptr;

    // Derive the operation name from the binary operator. At the moment we only
    // support '+' and '*'.
    switch (binop.getOp()) {
        case '+':
            return mlir::toy::AddOp::create(builder, loc, lhs, rhs);
        case '*':
            return mlir::toy::MulOp::create(builder, loc, lhs, rhs);
    }

    mlir::emitError(loc, "invalid binary operator '") << binop.getOp() << "'";
    return nullptr;
}

/// Emit a call expression. It emits specific operations for the `transpose`
/// builtin. Other identifiers are assumed to be user-defined functions.
mlir::Value MLIRGenImpl::mlirGen(CallExprAST &call) {
    std::string callee = call.getCallee();

    // Codegen the operands first.
    llvm::SmallVector<mlir::Value, 4> operands;
    for (auto &expr : *call.getArgs()) {
        auto arg = mlirGen(*expr);
        if (!arg)
            return nullptr;
        operands.push_back(arg);
    }

    // Builtin calls have their custom operation, meaning this is a
    // straightforward emission.
    if (callee == "transpose") {
        if (call.getArgs()->size() != 1) {
            mlir::emitError(loc, "MLIR codegen encountered an error: toy.transpose "
                            "does not accept multiple arguments");
            return nullptr;
        }
        return mlir::toy::TransposeOp::create(builder, loc, operands[0]);
    }

    // Otherwise this is a call to a user-defined function. Calls to
    // user-defined functions are mapped to a custom call that takes the callee
    // name as an attribute.
    return mlir::toy::GenericCallOp::create(builder, loc, callee, operands);
}

/// This is a reference to a variable in an expression. The variable is
/// expected to have been declared and so should have a value in the symbol
/// table, otherwise emit an error and return nullptr.
mlir::Value MLIRGenImpl::mlirGen(VariableExprAST &expr) {
    if (auto variable = symbolTable.lookup(expr.getName()))
        return variable;

    emitError(loc, "error: unknown variable '")
        << expr.getName() << "'";
    return nullptr;
}
