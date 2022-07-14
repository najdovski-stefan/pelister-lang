#pragma once

#include "ast.hpp"
#include <string>

class AstVisualizer {
public:
    void generateDot(const AstNode& root, const std::string& outputPath);

private:
    void generateDotRecursive(const AstNode& node, std::ostream& out);
    long nodeCounter = 0;
};
