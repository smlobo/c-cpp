//
// Created by Sheldon Lobo on 2/14/26.
//

#include <fstream>
#include <string>
#include <iostream>
#include <unistd.h>

#include <IR/Module.h>
#include <IR/PassManager.h>

#include <Transforms/InstCombine/InstCombine.h>
#include <Transforms/Scalar/Reassociate.h>
#include <Transforms/Scalar/GVN.h>
#include <Transforms/Scalar/SimplifyCFG.h>

#include <Passes/PassBuilder.h>

#include "codegen.h"
#include "lexer.h"
#include "parser.h"

std::ifstream inputFile;

std::unique_ptr<llvm::FunctionPassManager> TheFPM;
std::unique_ptr<llvm::LoopAnalysisManager> TheLAM;
std::unique_ptr<llvm::FunctionAnalysisManager> TheFAM;
std::unique_ptr<llvm::CGSCCAnalysisManager> TheCGAM;
std::unique_ptr<llvm::ModuleAnalysisManager> TheMAM;

std::filesystem::path fileName;
bool optimize = false;

static void printUsage(int argc, char *argv[]) {
    std::cerr << "Usage: " << argv[0] << " [-O] <file.ks>" << std::endl;
}

static bool parseOptions(int argc, char* argv[]) {
    int opt;
    while ((opt = getopt(argc, argv, "O")) != -1) {
        switch (opt) {
            case 'O':
                optimize = true;
                break;
            default:
                printUsage(argc, argv);
                return false;
        }
    }

    // Remaining arguments are positional
    if (optind >= argc) {
        std::cerr << "Error: missing input file\n";
        printUsage(argc, argv);
        return false;
    }

    fileName = argv[optind];

    // Enforce exactly one positional argument
    if (optind + 1 < argc) {
        std::cerr << "Error: too many input files\n";
        printUsage(argc, argv);
        return false;
    }

    // Strict naming of input file
    if (fileName.extension() != ".ks") {
        std::cerr << "File must be .ks\n";
        printUsage(argc, argv);
        return false;
    }

    return true;
}

static void InitializeModuleAndManagers(std::string namePrefix) {
    std::cerr << "Initializing module: " << namePrefix << std::endl;

    // Open a new context and module.
    TheContext = std::make_unique<llvm::LLVMContext>();
    TheModule = std::make_unique<llvm::Module>(namePrefix + ".ll", *TheContext);

    // Create a new builder for the module.
    Builder = std::make_unique<llvm::IRBuilder<>>(*TheContext);

    // Create new pass and analysis managers.
    TheFPM = std::make_unique<llvm::FunctionPassManager>();
    TheLAM = std::make_unique<llvm::LoopAnalysisManager>();
    TheFAM = std::make_unique<llvm::FunctionAnalysisManager>();
    TheCGAM = std::make_unique<llvm::CGSCCAnalysisManager>();
    TheMAM = std::make_unique<llvm::ModuleAnalysisManager>();
    // ThePIC = std::make_unique<PassInstrumentationCallbacks>();
    // TheSI = std::make_unique<StandardInstrumentations>(*TheContext,
    //                                                   /*DebugLogging*/ true);
    // TheSI->registerCallbacks(*ThePIC, TheMAM.get());

    // Add transform passes.
    // Do simple "peephole" optimizations and bit-twiddling optzns.
    TheFPM->addPass(llvm::InstCombinePass());
    // Reassociate expressions.
    TheFPM->addPass(llvm::ReassociatePass());
    // Eliminate Common SubExpressions.
    TheFPM->addPass(llvm::GVNPass());
    // Simplify the control flow graph (deleting unreachable blocks, etc).
    TheFPM->addPass(llvm::SimplifyCFGPass());

    // Register analysis passes used in these transform passes.
    llvm::PassBuilder PB;
    PB.registerModuleAnalyses(*TheMAM);
    PB.registerFunctionAnalyses(*TheFAM);
    PB.crossRegisterProxies(*TheLAM, *TheFAM, *TheCGAM, *TheMAM);
}

int main(int argc, char *argv[]) {
    if (!parseOptions(argc, argv)) {
        return 1;
    }
    inputFile = std::ifstream(fileName);
    if (!inputFile) {
        std::cerr << "Failed to open file: " << argv[1] << std::endl;
        return 1;
    }

    // Make the module, which holds all the code.
    InitializeModuleAndManagers(fileName.stem());

    getNextToken();
    bool done = false;
    while (!done) {
        switch (CurToken) {
            case tok_eof:
                done = true;
                break;
            case ';': // ignore top-level semicolons.
                getNextToken();
                break;
            case tok_def:
                HandleDefinition();
                break;
            case tok_extern:
                HandleExtern();
                break;
            default:
                HandleTopLevelExpression();
                break;
        }
    }

    // Write the module
    std::cerr << "Writing to file: " << TheModule->getName().str() << std::endl;
    std::error_code ec;
    llvm::raw_fd_ostream outputFile(TheModule->getName(), ec);
    if (ec) {
        std::cerr << "Error opening file: " << ec.message() << std::endl;
        return 1;
    }
    TheModule->print(outputFile, nullptr);

    return 0;
}
