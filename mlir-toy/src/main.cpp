//
// Created by Sheldon Lobo on 8/10/26.
//

#include "main.h"

#include <getopt.h>

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "mlir/IR/MLIRContext.h"
#include "mlir/Pass/PassManager.h"
#include "mlir/Transforms/Passes.h"
#include "llvm/Support/raw_ostream.h"

#include "ast.h"
#include "Dialect.h"
#include "lexer.h"
#include "MLIRGen.h"
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
    std::cerr << "Usage: " << argv[0] << " [-O] [-a] [-d] <file.mt>" << std::endl;
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

static int dumpMLIR(std::string namePrefix,
    std::vector<std::unique_ptr<FunctionAST>> &functions) {
    const std::string outputFile = namePrefix + ".mlir";
    std::cerr << "Writing module to: " << outputFile << std::endl;

    mlir::MLIRContext context;
    // Load our Dialect in this MLIR Context.
    context.getOrLoadDialect<mlir::toy::ToyDialect>();

    mlir::ModuleOp module = MLIRGenImpl(context).mlirGen(functions);
    if (!module)
        return 1;

    if (optimize) {
        mlir::PassManager pm(module->getName());
        // Apply any generic pass manager command line options and run the pipeline.
        // if (mlir::failed(mlir::applyPassManagerCLOptions(pm)))
        //     return 4;

        // Add a run of the canonicalizer to optimize the mlir module.
        pm.addNestedPass<mlir::toy::FuncOp>(mlir::createCanonicalizerPass());
        if (mlir::failed(pm.run(module)))
            return 4;
    }

    std::error_code error;
    llvm::raw_fd_ostream output(outputFile, error);
    if (error) {
        std::cerr << "Failed to open output file " << outputFile << ": "
                  << error.message() << std::endl;
        return 1;
    }

    module->print(output);
    output << '\n';
    return 0;
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

    getNextToken();
    bool done = false;
    std::vector<std::unique_ptr<FunctionAST>> functions;
    while (!done) {
        switch (CurToken) {
            case tok_eof:
                done = true;
                break;
            case ';': // ignore top-level semicolons.
                getNextToken();
                break;
            case tok_def:
                functions.push_back(HandleDefinition());
                break;
            case tok_extern:
                HandleExtern();
                break;
            default:
                std::cerr << "Unhandled top-level token: " << CurToken << std::endl;
                return 1;
        }
    }

    return dumpMLIR(fileName.stem(), functions);
}
