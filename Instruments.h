#ifndef MATH_INSTRUMENTS
#define MATH_INSTRUMENTS

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>
#include <math.h>

#include "color_print.h"
#include "TreeMemStruct/Tree.h"

#ifdef DEBUG
    #define ON_DEBUG(func) func
#else
    #define ON_DEBUG(func)
#endif

#define FREE(ptr) if(ptr) {free(ptr); ptr = NULL;}
#define ERR_PRINT(text) fprintf(stderr, RED_COLOR text RESET)

const double PRECISION = 1e-12;
const size_t ST_W_LEN = 5;

char* ReadFile(const char* const name);
char* SkipSpaces(char* ptr);
size_t GetWord(char** word, size_t len, char** cur_pos);
size_t CheckLen(char** word, size_t word_len, const size_t letter);
int DoubleCompare(double a, double b);
Operator_t FindOp(char* const word);
bool FindVar(Node_t* node, const char* const var);
bool FindOperator(char* word, Operator_t* op);
size_t FindVarPos(const char* const name, const Var_t* const vars, size_t vars_num);
TreeErr_t GetVarsValues(Tree_t* tree);
bool NeedBraces(Node_t* node, Node_t* next_node);
double CalcTree(Tree_t* tree);
double CalcTreeNode(const Node_t* node, const Var_t* vars, size_t vars_num  );

#endif // MATH_INSTRUMENTS
