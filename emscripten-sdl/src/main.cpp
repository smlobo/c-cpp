#include <iostream>

#include "sql-square.h"

int main(int argc, char *argv[]) {
    // Hack: Until I find a way to directly call sdl_square() from html
    if (argc > 10) {
        std::cout << "Calling sql_square with: " << argc << "\n";
        sdl_square(argc);
    } else {
        std::cout << "Skipping making square (" << argc << ")\n";
    }
    return 0;
}
