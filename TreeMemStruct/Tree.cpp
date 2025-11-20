#include "Tree.h"

Node_t* TreeNodeCtor(NodeType_t type, Value_t value, Node_t* left_som, Node_t* right_som)
{
    Node_t* node = (Node_t*) calloc(1, sizeof(Node_t));
    if(node == NULL) return NULL;

    node->node_type = type;
    node->value = value;
    node->prev_node = NULL;
    node->left = left_som;
    node->right = right_som;

    if (left_som)  left_som->prev_node  = &node;
    if (right_som) right_som->prev_node = &node;

    return node;
}


TreeErr_t TreeInsertLeft(Node_t* base_node, Node_t* inserting_node)
{
    assert(base_node != NULL);
    if (base_node->left != NULL)  return TREE_ERR_NODE_NOT_EMPTY;

    base_node->left = inserting_node;
    if (inserting_node) inserting_node->prev_node = &base_node;  // root = prev_node

    return TREE_OK;
}


TreeErr_t TreeInsertRight(Node_t* base_node, Node_t* inserting_node)
{
    assert(base_node != NULL);
    if (base_node->right != NULL) return TREE_ERR_NODE_NOT_EMPTY;

    base_node->right = inserting_node;
    if (inserting_node) inserting_node->prev_node = &base_node;

    return TREE_OK;
}


TreeErr_t TreeSortInsert(Node_t* root, Node_t* node)
{
    assert(root != NULL);
    assert(node != NULL);
    assert(root->node_type == NUM);

    while(1)
    {
        if(node->value.num <= root->value.num)
        {
            if (root->left == node) return TREE_ERR_DUPLICATE_NODE;
            if (TreeInsertLeft(root, node) == TREE_OK) break;
            else root = root->left;
        }
        else
        {
            if (root->right == node) return TREE_ERR_DUPLICATE_NODE;
            if(TreeInsertRight(root, node) == TREE_OK) break;
            else root = root->right;
        }
    }

    return TREE_OK;
}


TreeErr_t DeleteTreeNode(Node_t** node)
{
    assert(node != NULL);
    if (*node == NULL) return NULL_NODE;

    Node_t* cur_node = *node;

    if (cur_node->left)  DeleteTreeNode(&cur_node->left);
    if (cur_node->right) DeleteTreeNode(&cur_node->right);

    switch(cur_node->node_type)
    {
        case NUM:
            cur_node->value.num = 0;
            break;
        case VAR:
            free(cur_node->value.var);
            cur_node->value.var = NULL;
            break;
        case OP:
            cur_node->value.op = ADD;
            break;
        default:
            return INCORRECT_TYPE;
    }

    if (cur_node->prev_node) *(cur_node->prev_node) = NULL;

    free(cur_node);
    *node = NULL;

    return TREE_OK;
}


TreeErr_t PrintTreeNode(FILE* stream, const Node_t* node, TraverseMode_t mode)
{
    assert(mode == PREORDER || mode == INORDER || mode == POSTORDER);
    if (!node)
    {
        fprintf(stream, " nil ");
        return TREE_OK;
    }

    fprintf(stream, "(");
    switch(mode)
    {
        case PREORDER:
            PrintTreeData(stream, node);
            PrintTreeNode(stream, node->left,  mode);
            PrintTreeNode(stream, node->right, mode);
            break;
        case INORDER:
            PrintTreeNode(stream, node->left,  mode);
            PrintTreeData(stream, node);
            PrintTreeNode(stream, node->right, mode);
            break;
        case POSTORDER:
            PrintTreeNode(stream, node->left,  mode);
            PrintTreeNode(stream, node->right, mode);
            PrintTreeData(stream, node);
            break;
        default:
            return INVALID_MODE;
    }
    fprintf(stream, ")");

    return TREE_OK;
}


TreeErr_t PrintTreeData(FILE* stream, const Node_t* node)
{
    assert(stream != NULL);
    assert(node != NULL);

    switch(node->node_type)
    {
        case NUM:
            fprintf(stream, " %f ", node->value.num);
            break;
        case OP:
            fprintf(stream, " %d ", node->value.op);
            break;
        case VAR:
            fprintf(stream, " %s ", node->value.var); // пока просто печатаем номер переменной, а не ее название
            break;                                    // далее скорее всего будет массив с навзвниями...
        default:
            return INCORRECT_TYPE;
    }

    return TREE_OK;
}


TreeErr_t TreeDump_(const Node_t* node, int count_img, const char* func, const char* file, int line)
{
    ON_DEBUG(printf("node ptr = %p\n", node));
    if (node == NULL) return NULL_NODE;

    CreateDotFile(node);
    char command[MAX_FILE_NAME] = {};
    sprintf(command, "dot -Tsvg TreeMemStruct/svg_dot/dump.dot -o TreeMemStruct/svg_dot/%ddump.svg", count_img);
    system(command);

    WriteInHtmlFile(node, count_img, func, file, line);

    return TREE_OK;
}
