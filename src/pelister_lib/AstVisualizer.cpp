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
    outFile << "  node [shape=box, style=rounded];\n";
    outFile << "  edge [arrowhead=vee];\n";
    generateDotRecursive(root, outFile);
    outFile << "}\n";

    std::cout << "AST visualization saved to " << outputPath << std::endl;
    std::cout << "To generate a PNG, run: dot -Tpng " << outputPath << " -o ast.png" << std::endl;
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
    else if (auto ifNode = dynamic_cast<const IfNode*>(&node)) {
        long trueBranchId = nodeCounter;
        generateDotRecursive(ifNode->getTrueBranch(), out);
        out << "  node" << myId << " -> node" << trueBranchId << " [label=\"true\"];\n";

        if (ifNode->hasFalseBranch() && !ifNode->getFalseBranch().getNodes().empty()) {
            long falseBranchId = nodeCounter;
            generateDotRecursive(ifNode->getFalseBranch(), out);
            out << "  node" << myId << " -> node" << falseBranchId << " [label=\"false\"];\n";
        }
    }
    else if (auto funcDefNode = dynamic_cast<const FunctionDefinitionNode*>(&node)) {
        long bodyId = nodeCounter;
        generateDotRecursive(funcDefNode->getBody(), out);
        out << "  node" << myId << " -> node" << bodyId << " [label=\"body\"];\n";
    }
    else if (auto doLoopNode = dynamic_cast<const DoLoopNode*>(&node)) {
        long bodyId = nodeCounter;
        generateDotRecursive(doLoopNode->getBody(), out);
        out << "  node" << myId << " -> node" << bodyId << " [label=\"body\"];\n";
    }
}
