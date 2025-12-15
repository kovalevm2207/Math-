#ifndef DERIVATIVE
#define DERIVATIVE

#include "Instruments.h"
#include "PrintFunctions.h"

Node_t* TakeDerivative(FILE* file, Node_t* node, const char* const var);
Node_t* PowDerivative(FILE* file, Node_t* node, const char* const var);
Node_t* LogDerivative(FILE* file, Node_t* node, const char* const var);

#endif//DERIVATIVE
