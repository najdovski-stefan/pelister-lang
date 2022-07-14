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
    outFile << "  node [shape=box];\n";
    generateDotRecursive(root, outFile);
    outFile << "}\n";

    std::cout << "AST visualization saved to " << outputPath << std::endl;
}

void AstVisualizer::generateDotRecursive(const AstNode& node, std::ostream& out) {
    long myId = nodeCounter++;
    out << "  node" << myId << " [label=\"" << node.toString() << "\"];\n";

    if (auto binOp = dynamic_cast<const BinaryOpNode*>(&node)) {
        long leftId = nodeCounter;
        generateDotRecursive(binOp->getLeft(), out);
        out << "  node" << myId << " -> node" << leftId << ";\n";

        long rightId = nodeCounter;
        generateDotRecursive(binOp->getRight(), out);
        out << "  node" << myId << " -> node" << rightId << ";\n";
    }
}
