#ifndef MATH_TYPES
#define MATH_TYPES

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include "TreeMemStruct/Tree.h"
#include "color_print.h"
#include "Instruments.h"

#define FREE(ptr) if(ptr) {free(ptr); ptr = NULL;}

Node_t* GetTreeNode(char** cur_pos);
data_t* GetAndUnulyzeWord(char** cur_pos);

#endif // MATH_TYPES
