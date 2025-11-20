#ifndef TREE
#define TREE

#include "TreeBase.h"
#include "TreeDump.h"

Node_t* TreeNodeCtor(data_t* data, Node_t* left_som, Node_t* right_som);
Node_t* TreeNodeCtor_(NodeType_t type, Value_t value, Node_t* left_som, Node_t* right_som);
TreeErr_t TreeInsertLeft(Node_t* base_node, Node_t* inserting_node);
TreeErr_t TreeInsertRight(Node_t* base_node, Node_t* inserting_node);
TreeErr_t TreeSortInsert(Node_t* root, Node_t* node);
TreeErr_t DeleteTreeNode(Node_t** node);
TreeErr_t PrintTreeNode(FILE* stream, const Node_t* node, TraverseMode_t mode);
TreeErr_t PrintTreeData(FILE* stream, const Node_t* node);
TreeErr_t TreeDump_(const Node_t* node, int count_im, const char* func, const char* file, int line);

#define TreeDump(node, count_img) TreeDump_(node, count_img, __func__, __FILE__, __LINE__)

#endif // TREE
