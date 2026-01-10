#ifndef _AST_NODE_H_
#define _AST_NODE_H_

typedef enum {
    AST_SELECT = 0,
    AST_INSERT,
    AST_DELETE,
    AST_DROP,
    AST_UPDATE,
    AST_WHERE,
    AST_CONDITION,
    AST_AND,
    AST_OR
} aspNodeType;

#endif;
