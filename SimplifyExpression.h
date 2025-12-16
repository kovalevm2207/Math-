#ifndef SIMPLIFY_EXPRESSION
#define SIMPLIFY_EXPRESSION

#include "Instruments.h"
#include "PrintFunctions.h"

TreeErr_t SimplifyExpression(FILE* file, Node_t* base_tree, Node_t** tree, int* count_img, bool* is_print);
Node_t* ConstantFolding(Node_t** node_, bool* is_change);
Node_t* UnaryConstantFolding(Node_t** node_, bool* is_change);
Node_t* BinaryConstantFolding(Node_t** node_, bool* is_change);
Node_t* NeutralElementElimination(Node_t** node, bool* is_change);
Node_t* SimplifyAdd(Node_t** node_, bool* is_change);
Node_t* SimplifySub(Node_t** node_, bool* is_change);
Node_t* SimplifyMul(Node_t** node_, bool* is_change);
Node_t* SimplifyPow(Node_t** node_, bool* is_change);
Node_t* SimplifyLog(Node_t** node_, bool* is_change);
Node_t* SimplifyDiv(Node_t** node_, bool* is_change);

#endif//SIMPLIFY_EXPRESSION
