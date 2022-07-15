#include "AstVisualizer.hpp"
#include <fstream>
#include <iostream>

void AstVisualizer::generateDot(const AstNode& root, const std::string& outputPath) {
    std::ofstream outFile(outputPath);
    if (!outFile.is_open()) {
        std::cerr << "Error: Could not open file " << outputPath << std::endl;
        return;
    }

    outFile << "digraph AST {\n";
    outFile << "  rankdir=TB;\n";
    outFile << "  node [shape=box];\n";
    generateDotRecursive(root, outFile);
    outFile << "}\n";

    std::cout << "AST visualization saved to " << outputPath << std::endl;
}

void AstVisualizer::generateDotRecursive(const AstNode& node, std::ostream& out) {
    long myId = nodeCounter++;
    out << "  node" << myId << " [label=\"" << node.toString() << "\"];\n";

    if (auto programNode = dynamic_cast<const ProgramNode*>(&node)) {
        for (const auto& child : programNode->getNodes()) {
            long childId = nodeCounter;
            generateDotRecursive(*child, out);
            out << "  node" << myId << " -> node" << childId << ";\n";
        }
    }
}
