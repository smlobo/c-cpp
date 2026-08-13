//
// Created by Sheldon Lobo on 2/14/26.
//

#include <fstream>
#include <string>
#include <iostream>
#include <getopt.h>

#include <IR/PassManager.h>

#include <Passes/PassBuilder.h>

#include "ast.h"
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
bool debug = false;
bool ast_dump = false;

static struct option long_options[] = {
    {"help",      no_argument,       0, 'h'},
    {"optimize",  no_argument,       0, 'O'},
    {"ast-dump",  no_argument,       0, 'a'},
    {"debug",     no_argument,       0, 'd'},
    {0, 0, 0, 0}
};

static void printUsage(int argc, char *argv[]) {
    std::cerr << "Usage: " << argv[0] << " [-O] [-<file.ks>" << std::endl;
}

static bool parseOptions(int argc, char* argv[]) {
    int opt;
    while ((opt = getopt_long(argc, argv, "hOad", long_options, nullptr)) != -1) {
        switch (opt) {
            case 'h':
                printUsage(argc, argv);
                exit(0);
            case 'O':
                optimize = true;
                break;
            case 'd':
                debug = true;
                break;
            case 'a':
                ast_dump = true;
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
    if (fileName.extension() != ".mt") {
        std::cerr << "File must be .mt\n";
        printUsage(argc, argv);
        return false;
    }

    return true;
}

static void InitializeModuleAndManagers(std::string namePrefix) {
    std::cerr << "Initializing module: " << namePrefix << std::endl;
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
    std::unique_ptr<FunctionAST> currentFunctionAST = nullptr;
    while (!done) {
        switch (CurToken) {
            case tok_eof:
                done = true;
                break;
            case ';': // ignore top-level semicolons.
                getNextToken();
                break;
            case tok_def:
                currentFunctionAST = HandleDefinition();
                break;
            case tok_extern:
                HandleExtern();
                break;
            default:
                std::cerr << "Unhandled top-level token: " << CurToken << std::endl;
                return 1;
        }
    }

    return 0;
}
