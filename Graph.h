#ifndef GRAPH
#define GRAPH

#include "Instruments.h"
#include "PrintFunctions.h"

int MakeGraphScript(Node_t* first_func, Node_t* sec_func, const char* const img_name);
int DrawGraph(FILE* file, const char* const img_name, Node_t* first_func, Node_t* sec_func);

#endif//GRAPH
