//
// Created by Sheldon Lobo on 8/10/26.
//

#ifndef MLIR_TOY_SRC_MAIN_H
#define MLIR_TOY_SRC_MAIN_H

#include <memory>

#include "llvm/Analysis/CGSCCPassManager.h"
#include "llvm/Analysis/LoopAnalysisManager.h"
#include "llvm/IR/PassManager.h"

extern bool optimize;
extern bool debug;
extern bool ast_dump;

extern std::unique_ptr<llvm::FunctionPassManager> TheFPM;
extern std::unique_ptr<llvm::LoopAnalysisManager> TheLAM;
extern std::unique_ptr<llvm::FunctionAnalysisManager> TheFAM;
extern std::unique_ptr<llvm::CGSCCAnalysisManager> TheCGAM;
extern std::unique_ptr<llvm::ModuleAnalysisManager> TheMAM;

#endif // MLIR_TOY_SRC_MAIN_H
