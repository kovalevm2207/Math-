#ifndef MATH_PP
#define MATH_PP

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <string.h>

#include "Instruments.h"
#include "ReadingUserFile.h"
#include "PrintFunctions.h"
#include "Derivative.h"
#include "Graph.h"
#include "SimplifyExpression.h"

Tree_t* GetExpression(int* count_img, const char* const file_name);
void EndProgram(FILE* tex_file, Derivative_t* derivatives);

#endif // MATH_PP
