
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
    int ChildNodeCount;
    struct Token Token;
};

#endif