//
// Created by Sheldon Lobo on 2/14/26.
//

#include <fstream>
#include <string>
#include <iostream>
#include <IR/Module.h>

#include "codegen.h"
#include "lexer.h"
#include "parser.h"

extern void HandleDefinition();
extern void HandleExtern();
extern void HandleTopLevelExpression();

std::ifstream inputFile;

void printUsage(int argc, char *argv[]) {
    std::cerr << "Usage: " << argv[0] << " <file.ks>" << std::endl;
}


static void InitializeModule(std::string namePrefix) {
    std::cerr << "Initializing module: " << namePrefix << std::endl;

    // Open a new context and module.
    TheContext = std::make_unique<llvm::LLVMContext>();
    TheModule = std::make_unique<llvm::Module>(namePrefix + ".ll", *TheContext);

    // Create a new builder for the module.
    Builder = std::make_unique<llvm::IRBuilder<>>(*TheContext);
}

int main(int argc, char *argv[]) {
    // Expect a single file on the command line
    if (argc != 2) {
        printUsage(argc, argv);
        return 1;
    }
    std::filesystem::path fileName(argv[1]);
    if (fileName.extension() != ".ks") {
        std::cerr << "File must be .ks\n";
        return 1;
    }
    inputFile = std::ifstream(fileName);
    if (!inputFile) {
        std::cerr << "Failed to open file: " << argv[1] << std::endl;
        return 1;
    }

    // Make the module, which holds all the code.
    InitializeModule(fileName.stem());

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
