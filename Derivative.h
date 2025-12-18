#ifndef DERIVATIVE
#define DERIVATIVE

#include "Instruments.h"
#include "PrintFunctions.h"
#include "SimplifyExpression.h"
#include "Graph.h"

typedef struct
{
    Tree_t* tree;
    double data;
} Derivative_t;

const int MAX_DUMP_DERIVATIVE_ORDER = 2;

Derivative_t* GetNDerivatives(FILE* file, ProgramData_t input_data, int* count_img);
Node_t* TakeDerivative(FILE* file, Node_t* node, const char* const var, int* count_img, bool* is_print);
Node_t* PowDerivative(FILE* file, Node_t* node, const char* const var, int* count_img, bool* is_print);
Node_t* LogDerivative(FILE* file, Node_t* node, const char* const var, int* count_img, bool* is_print);

#endif//DERIVATIVE
