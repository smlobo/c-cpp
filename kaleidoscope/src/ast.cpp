//
// Created by Sheldon Lobo on 2/14/26.
//

#include <string>
#include <vector>

#include <IR/Constants.h>
#include <IR/Module.h>
#include <IR/Verifier.h>

#include "ast.h"
#include "codegen.h"
#include "errors.h"
#include "main.h"

//===----------------------------------------------------------------------===//
// Abstract Syntax Tree (aka Parse Tree)
//===----------------------------------------------------------------------===//

NumberExprAST::NumberExprAST(double Val) : Val(Val) {}

llvm::Value *NumberExprAST::codegen() {
    return llvm::ConstantFP::get(*TheContext, llvm::APFloat(Val));
}
VariableExprAST::VariableExprAST(const std::string &Name) : Name(Name) {}

llvm::Value *VariableExprAST::codegen() {
    // Look this variable up in the function.
    llvm::Value *V = NamedValues[Name];
    if (!V)
        LogErrorV("Unknown variable name");
    return V;
}

BinaryExprAST::BinaryExprAST(char Op, std::unique_ptr<ExprAST> LHS,
                             std::unique_ptr<ExprAST> RHS)
    : Op(Op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}

llvm::Value *BinaryExprAST::codegen() {
    llvm::Value *L = LHS->codegen();
    llvm::Value *R = RHS->codegen();
    if (!L || !R)
        return nullptr;

    switch (Op) {
        case '+':
            return Builder->CreateFAdd(L, R, "addtmp");
        case '-':
            return Builder->CreateFSub(L, R, "subtmp");
        case '*':
            return Builder->CreateFMul(L, R, "multmp");
        case '<':
            L = Builder->CreateFCmpULT(L, R, "cmptmp");
            // Convert bool 0/1 to double 0.0 or 1.0
            return Builder->CreateUIToFP(L, llvm::Type::getDoubleTy(*TheContext),
                                         "booltmp");
        default:
            return LogErrorV("invalid binary operator");
    }
}

CallExprAST::CallExprAST(const std::string &Callee,
                         std::vector<std::unique_ptr<ExprAST>> Args)
    : Callee(Callee), Args(std::move(Args)) {}

llvm::Value *CallExprAST::codegen() {
    // Look up the name in the global module table.
    llvm::Function *CalleeF = TheModule->getFunction(Callee);
    if (!CalleeF)
        return LogErrorV("Unknown function referenced");

    // If argument mismatch error.
    if (CalleeF->arg_size() != Args.size())
        return LogErrorV("Incorrect # arguments passed");

    std::vector<llvm::Value *> ArgsV;
    for (unsigned i = 0, e = Args.size(); i != e; ++i) {
        ArgsV.push_back(Args[i]->codegen());
        if (!ArgsV.back())
            return nullptr;
    }

    return Builder->CreateCall(CalleeF, ArgsV, "calltmp");
}

PrototypeAST::PrototypeAST(const std::string &Name, std::vector<std::string> Args)
    : Name(Name), Args(std::move(Args)) {}

const std::string &PrototypeAST::getName() const { return Name; }


llvm::Function *PrototypeAST::codegen() {
    // Make the function type:  double(double,double) etc.
    std::vector<llvm::Type *> Doubles(Args.size(), llvm::Type::getDoubleTy(*TheContext));
    llvm::FunctionType *FT =
            llvm::FunctionType::get(llvm::Type::getDoubleTy(*TheContext), Doubles, false);

    llvm::Function *F =
            llvm::Function::Create(FT, llvm::Function::ExternalLinkage, Name, TheModule.get());

    // Set names for all arguments.
    unsigned Idx = 0;
    for (auto &Arg : F->args())
        Arg.setName(Args[Idx++]);

    return F;
}

FunctionAST::FunctionAST(std::unique_ptr<PrototypeAST> Proto,
                         std::unique_ptr<ExprAST> Body)
    : Proto(std::move(Proto)), Body(std::move(Body)) {}

llvm::Function *FunctionAST::codegen() {
    // First, check for an existing function from a previous 'extern' declaration.
    llvm::Function *TheFunction = TheModule->getFunction(Proto->getName());

    if (!TheFunction)
        TheFunction = Proto->codegen();

    if (!TheFunction)
        return nullptr;

    // Create a new basic block to start insertion into.
    llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "entry", TheFunction);
    Builder->SetInsertPoint(BB);

    // Record the function arguments in the NamedValues map.
    NamedValues.clear();
    for (auto &Arg : TheFunction->args())
        NamedValues[std::string(Arg.getName())] = &Arg;

    if (llvm::Value *RetVal = Body->codegen()) {
        // Finish off the function.
        Builder->CreateRet(RetVal);

        // Validate the generated code, checking for consistency.
        verifyFunction(*TheFunction);

        // Optimize the function.
        if (optimize)
            TheFPM->run(*TheFunction, *TheFAM);

        return TheFunction;
    }

    // Error reading body, remove function.
    TheFunction->eraseFromParent();
    return nullptr;
}

IfExprAST::IfExprAST(std::unique_ptr<ExprAST> Cond, std::unique_ptr<ExprAST> Then,
                     std::unique_ptr<ExprAST> Else)
    : Cond(std::move(Cond)), Then(std::move(Then)), Else(std::move(Else)) {}

llvm::Value *IfExprAST::codegen() {
    llvm::Value *CondV = Cond->codegen();
    if (!CondV)
        return nullptr;

    // Convert condition to a bool by comparing non-equal to 0.0.
    CondV = Builder->CreateFCmpONE(CondV, llvm::ConstantFP::get(*TheContext,
        llvm::APFloat(0.0)), "ifcond");

    llvm::Function *TheFunction = Builder->GetInsertBlock()->getParent();
    // Create blocks for the then and else cases.  Insert the 'then' block at the
    // end of the function.
    llvm::BasicBlock *thenBB = llvm::BasicBlock::Create(*TheContext, "then",
        TheFunction);
    llvm::BasicBlock *elseBB = llvm::BasicBlock::Create(*TheContext, "else");
    llvm::BasicBlock *endifBB = llvm::BasicBlock::Create(*TheContext, "endif");

    Builder->CreateCondBr(CondV, thenBB, elseBB);

    // Then code
    Builder->SetInsertPoint(thenBB);
    llvm::Value *ThenV = Then->codegen();
    if (!ThenV)
        return nullptr;
    Builder->CreateBr(endifBB);
    // Codegen of 'Then' can change the current block, update thenBB for the PHI.
    thenBB = Builder->GetInsertBlock();

    // Else code
    TheFunction->insert(TheFunction->end(), elseBB);
    Builder->SetInsertPoint(elseBB);
    llvm::Value *ElseV = Else->codegen();
    if (!ElseV)
        return nullptr;
    Builder->CreateBr(endifBB);
    // Codegen of 'Else' can change the current block, update elseBB for the PHI.
    elseBB = Builder->GetInsertBlock();

    // endif PHI code
    TheFunction->insert(TheFunction->end(), endifBB);
    Builder->SetInsertPoint(endifBB);

    llvm::PHINode *PN = Builder->CreatePHI(llvm::Type::getDoubleTy(*TheContext),
        2, "iftmp");

    PN->addIncoming(ThenV, thenBB);
    PN->addIncoming(ElseV, elseBB);
    return PN;
}

ForExprAST::ForExprAST(std::string &VarName, std::unique_ptr<ExprAST> Start,
                       std::unique_ptr<ExprAST> End, std::unique_ptr<ExprAST> Stride,
                       std::unique_ptr<ExprAST> Body)
    : VarName(VarName), Start(std::move(Start)), End(std::move(End)),
      Stride(std::move(Stride)), Body(std::move(Body)) {}

llvm::Value *ForExprAST::codegen() {
    // Emit the start code first, without 'variable' in scope.
    llvm::Value *StartVal = Start->codegen();
    if (!StartVal)
        return nullptr;

    llvm::BasicBlock *loopHeaderBB = Builder->GetInsertBlock();
    llvm::Function *TheFunction = loopHeaderBB->getParent();

    // Create block for the loop body
    llvm::BasicBlock *loopBodyBB = llvm::BasicBlock::Create(*TheContext, "loopbody", TheFunction);

    // Explicit fallthru to loop body
    Builder->CreateBr(loopBodyBB);

    // Set insertion of the loop body
    Builder->SetInsertPoint(loopBodyBB);

    // Phi for loop index
    llvm::PHINode *loopIndex = Builder->CreatePHI(llvm::Type::getDoubleTy(*TheContext),
    2, "loopindex");
    loopIndex->addIncoming(StartVal, loopHeaderBB);

    // Within the loop, the variable is defined equal to the PHI node.  If it
    // shadows an existing variable, we have to restore it, so save it now.
    llvm::Value *OldVal = NamedValues[VarName];
    NamedValues[VarName] = loopIndex;

    // Emit the loop body
    if (!Body->codegen())
        return nullptr;

    // Emit the stride
    llvm::Value *StrideVal = Stride->codegen();
    if (!StrideVal)
        return nullptr;

    llvm::Value *NextVal = Builder->CreateFAdd(loopIndex, StrideVal, "nextloopindex");

    // Compute the end condition.
    llvm::Value *EndCond = End->codegen();
    if (!EndCond)
        return nullptr;

    // Convert condition to a bool by comparing non-equal to 0.0.
    EndCond = Builder->CreateFCmpONE(
        EndCond, llvm::ConstantFP::get(*TheContext, llvm::APFloat(0.0)), "loopcond");

    // loopBodyBB could be changed by codegen. Get if to complete the 'loopindex' Phi node
    llvm::BasicBlock *loopBodyEndBB = Builder->GetInsertBlock();
    loopIndex->addIncoming(NextVal, loopBodyEndBB);

    // Create block for the loop exit
    llvm::BasicBlock *loopExitBB = llvm::BasicBlock::Create(*TheContext, "loopexit", TheFunction);

    // Insert the conditional branch into the end of LoopEndBB.
    Builder->CreateCondBr(EndCond, loopBodyBB, loopExitBB);

    // Any new code will be inserted in AfterBB.
    Builder->SetInsertPoint(loopExitBB);

    // Restore the unshadowed variable.
    if (OldVal)
        NamedValues[VarName] = OldVal;
    else
        NamedValues.erase(VarName);

    // for expr always returns 0.0.
    return llvm::Constant::getNullValue(llvm::Type::getDoubleTy(*TheContext));
}
