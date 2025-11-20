#ifndef TREE_DUMP
#define TREE_DUMP

#include <time.h>
#include <stdint.h>

#include "TreeBase.h"
#include "Tree.h"

const int MAX_FILE_NAME = 100;

FILE* StartHTMLfile(void);
TreeErr_t CreateDotFile(const Node_t* node);
void MakeNodes(const Node_t* node, int* node_count, FILE* file);
const char* GenerateColor(const void* ptr);
void MakeArrows(const Node_t* node, int* node_count, FILE* file);
TreeErr_t WriteInHtmlFile(const Node_t* node, int count_img, const char* func, const char* file, int line);
int EndHTMLfile(void);

#define STRING(text) #text

#endif // TREE_DUMP
