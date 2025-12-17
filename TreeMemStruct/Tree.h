#ifndef TREE
#define TREE

#include "TreeBase.h"
#include "TreeDump.h"

Tree_t* TreeCtor(Node_t* root);
size_t GetTreeDepth(const Node_t* const node);
size_t CountTreeSize(const Node_t* const node);
size_t GetTreeVars(Var_t** vars, const Node_t* const node, size_t* cur_vars_num, size_t* max_vars_num);
bool FindVar(const char* const var, const Var_t* const vars, const size_t vars_num);
TreeErr_t MakePrevNode(Node_t* node);
TreeErr_t TreeDtor(Tree_t** tree);
TreeErr_t TreeStructDumpF(FILE* stdout, Tree_t* tree);
#define TreeStructDump(tree) TreeStructDumpF(stdout, tree)

Node_t* TreeNodeCtor(data_t* data, Node_t* left_som, Node_t* right_som);
Node_t* TreeNodeCtor_(NodeType_t type, Value_t value, Node_t* left_som, Node_t* right_som);
Node_t* DeepNodeCopy(Node_t* node);
TreeErr_t TreeInsertLeft(Node_t* base_node, Node_t* inserting_node);
TreeErr_t TreeInsertRight(Node_t* base_node, Node_t* inserting_node);
TreeErr_t TreeSortInsert(Node_t* root, Node_t* node);
TreeErr_t DeleteTreeNode(Node_t** node);
TreeErr_t PrintTreeNode(FILE* stream, const Node_t* node, TraverseMode_t mode);
TreeErr_t PrintTreeData(FILE* stream, const Node_t* node);
TreeErr_t TreeDump_(FILE* tex_file, const Node_t* node, int count_im, const char* func, const char* file, int line);

#define TreeDump(file, node, count_img) TreeDump_(file, node, count_img, __func__, __FILE__, __LINE__)

#endif // TREE
