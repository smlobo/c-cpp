//
// Created by Sheldon Lobo on 2/15/26.
//

#ifndef KALEIDOSCOPE_MAIN_H
#define KALEIDOSCOPE_MAIN_H

#include <Analysis/CGSCCPassManager.h>
#include <Analysis/LoopAnalysisManager.h>

extern std::unique_ptr<llvm::FunctionPassManager> TheFPM;
extern std::unique_ptr<llvm::LoopAnalysisManager> TheLAM;
extern std::unique_ptr<llvm::FunctionAnalysisManager> TheFAM;
extern std::unique_ptr<llvm::CGSCCAnalysisManager> TheCGAM;
extern std::unique_ptr<llvm::ModuleAnalysisManager> TheMAM;

#endif //KALEIDOSCOPE_MAIN_H