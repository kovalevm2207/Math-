#ifndef DERIVATIVE
#define DERIVATIVE

#include "Instruments.h"
#include "PrintFunctions.h"
#include "SimplifyExpression.h"

Node_t* TakeDerivative(FILE* file, Node_t* node, const char* const var, int* count_img, bool* is_print);
Node_t* PowDerivative(FILE* file, Node_t* node, const char* const var, int* count_img, bool* is_print);
Node_t* LogDerivative(FILE* file, Node_t* node, const char* const var, int* count_img, bool* is_print);

#endif//DERIVATIVE
