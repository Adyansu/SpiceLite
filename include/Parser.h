#pragma once
#include <string>
class Solver;
class Parser {
public:
    void parseFile(const std::string& filePath, Solver& solver) const;
    static double parseValue(const std::string& token);
};
