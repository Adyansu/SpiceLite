#include "Parser.h"
#include "Solver.h"

#include <exception>
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 2 || argc > 3) {
        std::cerr << "Usage: SpiceLite <netlist-file> [results-file]\n";
        return 1;
    }

    try {
        Solver solver;
        Parser parser;
        parser.parseFile(argv[1], solver);
        solver.solveConfigured();
        solver.printResults(std::cout);
        if (argc == 3) {
            solver.exportResults(argv[2]);
        }
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "SpiceLite error: " << error.what() << '\n';
        return 1;
    }
}
