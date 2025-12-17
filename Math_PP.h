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
#include "Teylor.h"

void EndProgram(FILE* tex_file, Derivative_t* derivatives, ProgramData_t input_data);

#endif // MATH_PP
