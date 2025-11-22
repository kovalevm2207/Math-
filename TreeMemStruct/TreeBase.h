#ifndef TREE_BASE
#define TREE_BASE

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "color_print.h"
// to do: struct Tree_s {ptr, max_size}

typedef enum
{
    NUM,
    VAR,
    OP
} NodeType_t; // да я знаю что это Node type type, и что

const char* const NodeTypes[] = {"NUM", "VAR", "OP"};

typedef enum
{
    ADD,
    SUB,
    MUL,
    DIV,
    POW,
    SQRT,
    SIN,
    COS,
    TG,
    CTG,
    SH,
    CH,
    TH,
    CTH,
    ARCSIN,
    ARCCOS,
    ARCTG,
    ARCCTG,
    LOG,
    LG,
    LN
} Operator_t;

const struct
{
    const char* name;
    const char* symbol;
    int         priority;
} Operators[] =
{
    {"ADD",    "+" ,    0},
    {"SUB",    "-" ,    1},
    {"MUL",    "*" ,    2},
    {"DIV",    "/" ,    2},
    {"POW",    "^" ,    2},
    {"SQRT",   "sqrt",  3},
    {"SIN",    "sin",   3},
    {"COS",    "cos",   3},
    {"TG",     "tg",    3},
    {"CTG",    "ctg",   3},
    {"SH",     "sh",    3},
    {"CH",     "ch",    3},
    {"TH",     "th",    3},
    {"CTH",    "cth",   3},
    {"ARCSIN", "arcsin",3},
    {"ARCCOS", "arccos",3},
    {"ARCTG",  "arctg", 3},
    {"ARCCTG", "arcctg",3},
    {"LOG",    "log",   3},
    {"LG",     "lg",    3},
    {"LN",     "ln",    3},
};

typedef union
{
    double num;
    char* var;
    Operator_t  op;
} Value_t;

typedef struct Node_t
{
    struct Node_t** prev_node;
    Value_t   value;
    NodeType_t node_type;
    struct Node_t*  left;
    struct Node_t*  right;
} Node_t;

typedef enum
{
    TREE_OK                 = 1 << 0,
    CALLLOC_ERR             = 1 << 1,
    TREE_ERR_NODE_NOT_EMPTY = 1 << 2,
    NULL_FILE               = 1 << 3,
    END_FILE_ERR            = 1 << 4,
    TREE_ERR_DUPLICATE_NODE = 1 << 5,
    NULL_NODE               = 1 << 6,
    INVALID_MODE            = 1 << 7,
    INCORRECT_TYPE          = 1 << 8
} TreeErr_t;

typedef enum
{
    PREORDER  = 0,
    INORDER   = 1,
    POSTORDER = 2
} TraverseMode_t;

typedef struct
{
    NodeType_t type;
    Value_t value;
} data_t;

#ifdef DEBUG
    #define ON_DEBUG(func) func
    #define CHECK_PTR(param, name) if (param == NULL)        \
                                    {                        \
                                        return NULL_##name;  \
                                    }
#else
    #define ON_DEBUG(func)
    #define CHECK_PTR(param, name)
#endif // DEBUG


#endif // TREE_BASE
