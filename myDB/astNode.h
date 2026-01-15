#ifndef _AST_NODE_H_
#define _AST_NODE_H_

typedef enum {
    AST_SELECT = 0,
    AST_INSERT,
    AST_DELETE,
    AST_DROP,
    AST_UPDATE,
    AST_ROW,
    AST_COLUMN,
    AST_VALUE,
    AST_WHERE,
    AST_CONDITION,
    AST_AND,
    AST_OR
} astNodeType;

typedef struct astNode
{
    astNodeType type;
    struct astNode* left;
    struct astNode* right;
    char* table;
    char* column;
    char* value;
    int op;
}astNode;

astNode* createAstNode(astNodeType  type);

astNode* buildColumnList(const char** columns, int size);
astNode* buildValuesList(const char*** values, int rows, int cols);

int astListLenght(astNode * node);
astNode* astListAt(astNode* node, int index);

void freeAstNode(astNode* node);


#endif;
