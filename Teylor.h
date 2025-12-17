#ifndef TAYLOR
#define TAYLOR

#include "Instruments.h"
#include "Derivative.h"
#include "ReadingUserFile.h"

Tree_t* TaylorExpansion(FILE* file, Derivative_t* derivatives);
Tree_t* MakeTaylorTree(Derivative_t* derivatives);
int DrawTaylorGraph(FILE* file, const char* const img_name, Tree_t* base_func, Node_t* taylor_func, Node_t* tangent_line);
Tree_t* TangentLine(double k, double b, double x_0, double y_0);

#endif//TAYLOR
