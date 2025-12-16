#include "Derivative.h"


Derivative_t* GetNDerivatives(FILE* file, Tree_t* user_tree, int* count_img)
{
    assert(user_tree);
    assert(count_img);
    assert(file);

    Derivative_t* derivatives = (Derivative_t*) calloc(TEYLOR_ORDER + 1, sizeof(Derivative_t));
    derivatives[0].tree = user_tree;

    GetVarsValues(user_tree);
    ON_DEBUG(TreeStructDump(user_tree));

    bool is_print = true;
    for(int derivative_order = 1; derivative_order <= TEYLOR_ORDER; derivative_order++)
    {
        if(derivative_order > 1) fprintf(file, "Идем дальше...\n\n");
        if(derivative_order <= MAX_DUMP_DERIVATIVE_ORDER)
            PrintDerivativeBegining(file);

        Node_t* derivative = TakeDerivative(file, derivatives[derivative_order - 1].tree->root, "x", count_img, &is_print);
        MakePrevNode(derivative);
        derivatives[derivative_order].tree = TreeCtor(derivative);
        TreeDump(derivative, (*count_img)++);

        if(derivatives[derivative_order].tree->size < MAX_DUMP_SIZE)
        {
            char* img_name = (char*) calloc(8, sizeof(char));
            sprintf(img_name, "graph%d", derivative_order);
            DrawGraph(file, img_name, derivatives[derivative_order - 1].tree->root, derivatives[derivative_order].tree->root);
            free(img_name);
        }
        else
        {
            fprintf(file, "\\section*{Построение графика функций}\n\n"
                              "Сказал же, по аналогии. Сами дальше стройте!");
        }
        for(size_t cur_var = 0; cur_var < derivatives[derivative_order].tree->vars_num; cur_var++)
            derivatives[derivative_order].tree->vars[cur_var] = derivatives[0].tree->vars[cur_var];

        derivatives[derivative_order].data = CalcTreeNode(derivatives[derivative_order].tree->root,
                                                          derivatives[0].tree->vars,
                                                          derivatives[0].tree->vars_num);
        is_print = true;
    }

    return derivatives;
}
Node_t* TakeDerivative(FILE* file, Node_t* node, const char* const var, int* count_img, bool* is_print)
{
    assert(var);
    assert(node);
    assert(var);
    assert(count_img);
    assert(is_print);

    #include "DerivativeDSL.h"
    Node_t* new_node = NULL;

    switch(node->node_type)
    {
        default: return NULL;
        case NUM:
            new_node = n(0);
            break;
        case VAR:
            if(strcmp(node->value.var, var) == 0) new_node = n(1);
            else                                  new_node = n(0);
            break;
        case  OP:
            Node_t* left = node->left, *right = node->right;
            switch(node->value.op)
            {
                case NOT_OP:
                default: return NULL;
                case ADD:       new_node = ADD_(d(L), d(R));                                                   break;  //  d(a) + d(b)
                case SUB:       new_node = SUB_(d(L), d(R));                                                   break;  //  d(a) - d(b)
                case MUL:       new_node = ADD_(MUL_(d(L), c(R)), MUL_(c(L), d(R)));                           break;  //  d(f)*g + f*d(g)
                case DIV:       new_node = DIV_(SUB_(MUL_(d(L), c(R)), MUL_(c(L), d(R))),POW_(c(R), n(2)));    break;  //  (d(f)*g - f*d(g))/(g)^2
                case SQRT:      new_node = COMP_FUNC(DIV_(n(1), MUL_(n(2), SQRT_(c(L)))));                     break;  //  1/(2 * √a) * d(a)
                case SIN:       new_node = COMP_FUNC(COS_(c(L)));                                              break;  //  cos(a) * d(a)
                case COS:       new_node = COMP_FUNC(MUL_(n(-1), SIN_(c(L))));                                 break;  // -sin(a) * d(a)
                case TG:        new_node = COMP_FUNC(DIV_(n(1), COS_(c(L))));                                  break;  //  1/cos^2(a) * d(a)
                case CTG:       new_node = COMP_FUNC(MUL_(n(-1), DIV_(n(1), SIN_(c(L)))));                     break;  // -1/sin^2(a) * d(a)
                case SH:        new_node = COMP_FUNC(CH_(c(L)));                                               break;  //  ch(a) * d(a)
                case CH:        new_node = COMP_FUNC(SH_(c(L)));                                               break;  //  sh(a) * d(a)
                case TH:        new_node = COMP_FUNC(DIV_( n(1), POW_(CH_(c(L)), n(2))));                      break;  //  1/ch^2(a) * d(a)
                case CTH:       new_node = COMP_FUNC(DIV_(n(-1), POW_(SH_(c(L)), n(2))));                      break;  // -1/sh^2(a) * d(a)
                case ARCSIN:    new_node = COMP_FUNC(DIV_( n(1), SQRT_(SUB_(n(1), POW_(c(L), n(2))))));        break;  //  1/√(1 - a^2)) * d(a)
                case ARCCOS:    new_node = COMP_FUNC(DIV_(n(-1), SQRT_(SUB_(n(1), POW_(c(L), n(2))))));        break;  // -1/√(1 - a^2)) * d(a)
                case ARCTG:     new_node = COMP_FUNC(DIV_( n(1), ADD_(n(1), POW_(c(L), n(2)))));               break;  //  1/(1 + a^2) * d(a)
                case ARCCTG:    new_node = COMP_FUNC(DIV_(n(-1), ADD_(n(1), POW_(c(L), n(2)))));               break;  // -1/(1 + a^2) * d(a)
                case LG:        new_node = COMP_FUNC(DIV_(n(1), MUL_(LN_(n(10)), c(L))));                      break;  //  1/(ln(10) * a) * d(a)
                case LN:        new_node = COMP_FUNC(DIV_(n(1), c(L)));                                        break;  //  1/a * d(a)
                case LOG:       new_node = LogDerivative(file, node, var, count_img, is_print);                break;
                case POW:       new_node = PowDerivative(file, node, var, count_img, is_print);                break;
            }
            break;
    }

    if(CountTreeSize(new_node) < MAX_DUMP_SIZE && *is_print == true)
    {
        fprintf(file, "\\subsubsection*{Дифференцируем:}\n\n%s\n", TakeDerivativePhrases[(long unsigned int)rand() % TAKE_DERIVATIVE_PHRASES_NUM]);
        fprintf(file, "\\begin{dmath}"
                      "\\frac{d}{d%s}\\left(", var);
        WriteTreeNodeLaTeX(file, node);
        fprintf(file, "\\right)=");
        WriteTreeNodeLaTeX(file, new_node);
        fprintf(file, "\\end{dmath}\n");

        PrintBeginSimplify(file);
    }
    else
    {
        if (*is_print == true) fprintf(file, "\\subsubsection*{Дальнейшее по аналогии...}\n\n");
        *is_print = false;
    }
    Node_t* copy = c(new_node);
    SimplifyExpression(file, copy, &new_node, count_img, is_print);
    DeleteTreeNode(&copy);
    #include "UndefDerivativeDSL.h"
    return new_node;
}
Node_t* PowDerivative(FILE* file, Node_t* node, const char* const var, int* count_img, bool* is_print)
{
    assert(file);
    assert(node);
    assert(var);

    Node_t* left = node->left, *right = node->right;
    #include "DerivativeDSL.h"
    bool left_flag  = FindVar(left,  var);
    bool right_flag = FindVar(right, var);

         if(left_flag && !right_flag) return COMP_FUNC(MUL_(c(R), POW_(c(L), SUB_(c(R), n(1)))));  //  n * a^(n - 1) * d(a)
    else if(!left_flag && right_flag) return MUL_(MUL_(POW_(c(L), c(R)), LN_(c(L))), d(R));  //  n^a * ln(n) * d(a)
    else if(left_flag && right_flag)
    {
        Node_t* auxiliary_transformation = MUL_(LN_(c(L)), c(R));  // ln(a) * b) =
        Node_t* new_node = MUL_(c(node), d(auxiliary_transformation)); // a^b * d(ln(a) * b)
        DeleteTreeNode(&auxiliary_transformation);
        return new_node;
    }
    else return n(0);  // 0
}
Node_t* LogDerivative(FILE* file, Node_t* node, const char* const var, int* count_img, bool* is_print)
{
    assert(file);
    assert(node);
    assert(var);

    Node_t* left = node->left, *right = node->right;
    #include "DerivativeDSL.h"
    bool left_flag  = FindVar(left,  var);
    bool right_flag = FindVar(right, var);

    if (left_flag && !right_flag) return COMP_FUNC(DIV_(n(1), MUL_(LN_(c(R)), c(L)))); //  1/(ln(b) * a) * d(a)
    else if(!left_flag && right_flag)
    {
        Node_t* auxiliary_transformation = DIV_(n(1), LOG_(c(R), c(L)));  //  d(1/log_a(b))
        Node_t* new_node = d(auxiliary_transformation);
        DeleteTreeNode(&auxiliary_transformation);
        return new_node;
    }
    else if(left_flag && right_flag)
    {
        Node_t* auxiliary_transformation = DIV_(LN_(c(L)), LN_(c(R)));  // d(ln(a)/ln(b))
        Node_t* new_node = d(auxiliary_transformation);
        DeleteTreeNode(&auxiliary_transformation);
        return new_node;
    }
    else return n(0);  // 0
}
