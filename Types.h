#ifndef MATH_TYPES
#define MATH_TYPES

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <string.h>

#include "TreeMemStruct/Tree.h"
#include "color_print.h"
#include "Instruments.h"

#define FREE(ptr) if(ptr) {free(ptr); ptr = NULL;}

Node_t* GetTreeNode(char** cur_pos);
data_t* GetAndUnulyzeWord(char** cur_pos, data_t* data);
data_t* UnulyzeWord(char* word, data_t* data);
bool FindOperator(char* word, Operator_t* op);

#endif // MATH_TYPES
