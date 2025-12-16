#include "SimplifyExpression.h"

TreeErr_t SimplifyExpression(FILE* file, Node_t* base_tree, Node_t** tree, int* count_img, bool* is_print)
{
    assert(tree);
    assert(*tree);
    assert(base_tree);
    assert(count_img);
    assert(file);
    assert(is_print);

    Node_t* result = NULL;
    bool is_change_CF = false;
    bool is_change_NEE = false;

    do
    {
        is_change_CF = false;
        is_change_NEE = false;

        result = ConstantFolding(tree, &is_change_CF);
        assert(result);
        *tree = result;
        if(is_change_CF)
        {
            TreeDump(*tree, (*count_img)++);
            if(CountTreeSize(base_tree) < MAX_DUMP_SIZE && *is_print == true)
                PrintSimplifyRes(file, base_tree, *tree, "x", is_change_NEE);
            else *is_print = false;
        }

        result = NeutralElementElimination(tree, &is_change_NEE);
        assert(result);
        *tree = result;
        if(is_change_NEE)
        {
            TreeDump(*tree, (*count_img)++);
            if(CountTreeSize(base_tree) < MAX_DUMP_SIZE && *is_print == true)
                PrintSimplifyRes(file, base_tree, *tree, "x", is_change_NEE);
            else *is_print = false;
        }
    }
    while(is_change_NEE || is_change_CF);

    if(*is_print == true) fprintf(file, "%s\n\n", CanNotSimplifyPhrases[(long unsigned int)rand() % CAN_NOT_SIMPLIFY_PHRASES_NUM]);
    return TREE_OK;
}
Node_t* ConstantFolding(Node_t** node_, bool* is_change)
{
    assert(is_change);
    assert(node_);
    Node_t* node = *node_;
    if(!node) return NULL;

    if(node->node_type == VAR || node->node_type == NUM) return node;
    assert(node->node_type == OP);

    Node_t* new_node = NULL;
    Node_t* new_left  = ConstantFolding(&node->left, is_change);
    Node_t* new_right = ConstantFolding(&node->right, is_change);
    assert(new_left);

    switch(node->value.op)
    {
        case NOT_OP:
        default:
            ERR_PRINT("INVALID_OP\n in");
            return NULL;
        case ADD: case SUB: case MUL:
        case DIV: case POW: case LOG:
        {
            assert(new_right);
            TreeInsertLeft(node, new_left);
            TreeInsertRight(node, new_right);
            new_node = BinaryConstantFolding(node_, is_change);
            break;
        }
        case TG:     case SH:     case CH:
        case TH:     case LG:     case LN:
        case SIN:    case COS:    case CTG:
        case CTH:    case SQRT:   case ARCTG:
        case ARCSIN: case ARCCOS: case ARCCTG:
        {
            TreeInsertLeft(node, new_left);
            new_node = UnaryConstantFolding(node_, is_change);
            break;
        }
    }
    assert(new_node);
    return new_node;
}
Node_t* UnaryConstantFolding(Node_t** node_, bool* is_change)
{
    assert(is_change);
    assert(node_);
    assert(*node_);
    assert((*node_)->node_type == OP);

    Node_t* node = *node_;
    Node_t* left = node->left;
    assert(left);

    if(left->node_type == VAR || left->node_type == OP) return node;
    assert(left->node_type == NUM);

    #define LV left->value.num
    #include "DerivativeDSL.h"

    Node_t* new_node = NULL;
    switch(node->value.op)
    {
        case SQRT:   new_node = n(sqrt(LV));        break;
        case SIN:    new_node = n(sin(LV));         break;
        case COS:    new_node = n(cos(LV));         break;
        case TG:     new_node = n(tan(LV));         break;
        case CTG:    new_node = n(1/tan(LV));       break;
        case SH:     new_node = n(sinh(LV));        break;
        case CH:     new_node = n(cosh(LV));        break;
        case TH:     new_node = n(tanh(LV));        break;
        case CTH:    new_node = n(1/tanh(LV));      break;
        case ARCSIN: new_node = n(asin(LV));        break;
        case ARCCOS: new_node = n(acos(LV));        break;
        case ARCTG:  new_node = n(atan(LV));        break;
        case ARCCTG: new_node = n(M_PI_2-atan(LV)); break;
        case LG:     new_node = n(log10(LV));       break;
        case LN:     new_node = n(log(LV));         break;
        case ADD: case SUB: case MUL:
        case DIV: case POW: case LOG:
        case NOT_OP:
        default: return NULL;
    }

    #include "UndefDerivativeDSL.h"
    #undef LV

    assert(new_node);
    DeleteTreeNode(node_);
    *is_change = true;

    return new_node;
}
Node_t* BinaryConstantFolding(Node_t** node_, bool* is_change)
{
    assert(is_change);
    assert(node_);
    assert(*node_);
    assert((*node_)->node_type == OP);

    Node_t* node = *node_;
    Node_t* left = node->left, *right = node->right;
    assert(left); assert(right);

    if(left->node_type == VAR || left->node_type == OP) return node;
    assert(left->node_type == NUM);

    if(right->node_type == VAR || right->node_type == OP) return node;
    assert(right->node_type == NUM);

    #define LV left->value.num
    #define RV right->value.num
    #include "DerivativeDSL.h"

    if(node->node_type == VAR || node->node_type == NUM) return node;
    assert(node->node_type == OP);

    Node_t* new_node = NULL;
    switch(node->value.op)
    {
        case ADD: new_node = n(LV + RV);         break;
        case SUB: new_node = n(LV - RV);         break;
        case MUL: new_node = n(LV * RV);         break;
        case DIV: new_node = n(LV / RV);         break;
        case POW: new_node = n(pow(LV,RV));      break;
        case LOG: new_node = n(log(LV)/log(RV)); break;
        case TG:     case SH:     case CH:
        case TH:     case LG:     case LN:
        case SIN:    case COS:    case CTG:
        case CTH:    case SQRT:   case ARCTG:
        case ARCSIN: case ARCCOS: case ARCCTG:
        case NOT_OP:
        default: return NULL;
    }

    #include "UndefDerivativeDSL.h"
    #undef LV
    #undef RV

    assert(new_node);
    DeleteTreeNode(node_);
    *is_change = true;
    return new_node;
}
Node_t* NeutralElementElimination(Node_t** node_, bool* is_change)
{
    assert(is_change);
    assert(node_);
    Node_t* node = *node_;
    if(!node) return NULL;

    if(node->node_type == NUM || node->node_type == VAR) return node;
    assert(node->node_type == OP);

    TreeInsertLeft (node, NeutralElementElimination(&node->left,  is_change));
    TreeInsertRight(node, NeutralElementElimination(&node->right, is_change));

    switch(node->value.op)
    {
        case NOT_OP:
        default:
            ERR_PRINT("InvalidOperatorType\n");
            return NULL;
        case ADD: return SimplifyAdd(node_, is_change);
        case SUB: return SimplifySub(node_, is_change);
        case MUL: return SimplifyMul(node_, is_change);
        case POW: return SimplifyPow(node_, is_change);
        case LOG: return SimplifyLog(node_, is_change);
        case DIV: return SimplifyDiv(node_, is_change);
        case TG:     case SH:      case CH:
        case TH:     case LG:      case LN:
        case SIN:    case COS:     case CTG:
        case CTH:    case SQRT:    case ARCTG:
        case ARCSIN: case ARCCOS:  case ARCCTG:
                  return node;
    }
    return NULL;
}
Node_t* SimplifyAdd(Node_t** node_, bool* is_change)
{
    assert(node_); assert(*node_); assert(is_change);

    Node_t* node= *node_;
    assert(node->node_type == OP); assert(node->value.op == ADD);

    Node_t* left = node->left, *right= node->right;
    assert(node->left); assert(node->right);

    Node_t* const_node = NULL;
    if(left->node_type == NUM)
    {
        if(DoubleCompare(left->value.num, 0) == 0)  const_node = right;
        else return node;
    }
    else if(right->node_type == NUM)
    {
        if(DoubleCompare(right->value.num, 0) == 0) const_node = left;
        else return node;
    }
    else return node;

    Node_t* const_cpy = DeepNodeCopy(const_node);
    assert(const_cpy);
    DeleteTreeNode(node_);
    *is_change = true;
    return const_cpy;
}
Node_t* SimplifySub(Node_t** node_, bool* is_change)
{
    assert(node_); assert(*node_); assert(is_change);

    Node_t* node= *node_;
    assert(node->node_type == OP); assert(node->value.op == SUB);

    Node_t* left = node->left, *right= node->right;
    assert(node->left); assert(node->right);

    #include "DerivativeDSL.h"
    Node_t* simple_res = NULL;
    if(left->node_type == NUM) // 0 - expr = (-1) * expr
    {
        if(DoubleCompare(left->value.num, 0) == 0) simple_res = MUL_(n(-1), c(R));
        else return node;
    }
    else if(right->node_type == NUM) // expr - 0 = expr
    {
        if(DoubleCompare(right->value.num, 0) == 0) simple_res = c(L);
        else return node;
    }
    else return node;
    #include "UndefDerivativeDSL.h"

    assert(simple_res);
    DeleteTreeNode(node_);
    *is_change = true;
    return simple_res;
}
Node_t* SimplifyMul(Node_t** node_, bool* is_change)
{
    assert(node_); assert(*node_); assert(is_change);

    Node_t* node= *node_;
    assert(node->node_type == OP); assert(node->value.op == MUL);

    Node_t* left = node->left, *right= node->right;
    assert(node->left); assert(node->right);

    #include "DerivativeDSL.h"
    Node_t* simple_res = NULL;
    if(left->node_type == NUM)
    {
             if(DoubleCompare(left->value.num, 0) == 0) simple_res = n(0); // 0 * expr
        else if(DoubleCompare(left->value.num, 1) == 0) simple_res = c(R); // 1 * expr
        else return node;
    }
    else if(right->node_type == NUM)
    {
             if(DoubleCompare(right->value.num, 0) == 0) simple_res = n(0); // expr * 0
        else if(DoubleCompare(right->value.num, 1) == 0) simple_res = c(L); // expr * 1
        else return node;
    }
    else return node;
    #include "UndefDerivativeDSL.h"

    assert(simple_res);
    DeleteTreeNode(node_);
    *is_change = true;
    return simple_res;
}
Node_t* SimplifyPow(Node_t** node_, bool* is_change)
{
    assert(node_); assert(*node_); assert(is_change);

    Node_t* node= *node_;
    assert(node->node_type == OP); assert(node->value.op == POW);

    Node_t* left = node->left, *right= node->right;
    assert(node->left); assert(node->right);

    #include "DerivativeDSL.h"
    Node_t* simple_res = NULL;
    if(left->node_type == NUM)
    {
        if(DoubleCompare(left->value.num, 1) == 0) simple_res = n(1); // 1 ^ expr = 1
        else return node;
    }
    else if(right->node_type == NUM)
    {
             if(DoubleCompare(right->value.num, 0) == 0) simple_res = n(1); // expr ^ 0 = 1
        else if(DoubleCompare(right->value.num, 1) == 0) simple_res = c(L); // expr ^ 1 = expr
        else return node;
    }
    else return node;
    #include "UndefDerivativeDSL.h"

    assert(simple_res);
    DeleteTreeNode(node_);
    *is_change = true;
    return simple_res;
}
Node_t* SimplifyLog(Node_t** node_, bool* is_change)
{
    assert(node_); assert(*node_); assert(is_change);

    Node_t* node= *node_;
    assert(node->node_type == OP); assert(node->value.op == LOG);

    Node_t* left = node->left; assert(node->left);

    #include "DerivativeDSL.h"
    Node_t* simple_res = NULL;
    if(left->node_type == NUM)
    {
        if(DoubleCompare(left->value.num, 1) == 0) simple_res = n(0); // log(1, expr) = 0
        else return node;
    }
    else return node;
    #include "UndefDerivativeDSL.h"

    assert(simple_res);
    DeleteTreeNode(node_);
    *is_change = true;
    return simple_res;
}
Node_t* SimplifyDiv(Node_t** node_, bool* is_change)
{
    assert(node_); assert(*node_); assert(is_change);

    Node_t* node= *node_;
    assert(node->node_type == OP); assert(node->value.op == DIV);

    Node_t* left = node->left, *right= node->right;
    assert(node->left); assert(node->right);

    #include "DerivativeDSL.h"
    Node_t* simple_res = NULL;
    if(left->node_type == NUM)
    {
        if(DoubleCompare(left->value.num, 0) == 0) simple_res = n(0); // 0 / expr
        else return node;
    }
    else if(right->node_type == NUM)
    {
        if(DoubleCompare(right->value.num, 1) == 0) simple_res = c(L); // expr / 1
        else return node;
    }
    else return node;
    #include "UndefDerivativeDSL.h"

    assert(simple_res);
    DeleteTreeNode(node_);
    *is_change = true;
    return simple_res;
}
