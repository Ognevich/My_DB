#ifndef _AST_NODE_H_
#define _AST_NODE_H_
#include "parser_keywords.h"

typedef enum {
    AST_SELECT = 0,
    AST_INSERT,
    AST_CREATE_TABLE,
    AST_CREATE_DATABASE,
    AST_ROW,
    AST_COLUMN,
    AST_VALUE,
    AST_WHERE,
    AST_CONDITION,
    AST_TYPE,
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
    sqlValuesType valueType;
    int op;
}astNode;

astNode* createAstNode(astNodeType  type);

astNode* buildColumnList(const char** columns, int size);
astNode* buildValuesList(const parsedValue*** values, int rows, int cols);
astNode* buildCreateTableColumnsList(const char*** columns, int size);

int astListLenght(astNode * node);
astNode* astListAt(astNode* node, int index);
astNode* astLinkedListAt(astNode* node, int row, int col);

void freeAstNode(astNode* node);


#endif;
