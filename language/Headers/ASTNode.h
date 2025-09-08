
#include "Token.h"

#ifndef ASTNODE_H
#define ASTNODE_H

enum ASTNodeType {
    EmptyNode,
    LeafNode,
    AssigmentNode,
    OperatorNode,
    FunctionNode
};

struct ASTNode {
    enum ASTNodeType Type;
    struct ASTNode* ChildNodes;
    int ChildNodeCount; // Only used for certain Nodes where the amount of children is unknown such as function calls
    struct Token Token;
};

#endif