#include "Types.h"

int main()
{
    //SECTION - reading from file
    char* user_file = ReadFile("Expression.txt");
    assert(user_file && "NULL user_file, check ReadFile func");
    char* cur_pos = SkipSpaces(user_file);

    Node_t* user_tree = GetTreeNode(&cur_pos);
    FREE(user_file)

    StartHTMLfile();
    TreeDump(user_tree, 1);

    //SECTION - write in LaTeX
    srand((unsigned int) time(NULL));

    FILE* tex_file = fopen("LaTeX.tex","w");
    assert(tex_file);
    BeginLaTeXDocument(tex_file);

    DumpLaTeX(tex_file, user_tree);

    Node_t* copy = DeepNodeCopy(user_tree);

    TreeDump(copy, 2);
    DumpLaTeX(tex_file, copy);

    //SECTION - derivative
    Node_t* first_derivative = TakeDerivative(tex_file, user_tree, "x");
    TreeDump(first_derivative, 3);
    DumpLaTeX(tex_file, first_derivative);

    //SECTION - end of the program
    EndLaTeXDocument(tex_file);
    fclose(tex_file);
    tex_file = NULL;

    EndHTMLfile();

    DeleteTreeNode(&first_derivative);
    DeleteTreeNode(&copy);
    DeleteTreeNode(&user_tree);
    return 0;
}


//SECTION -  reading
Node_t* GetTreeNode(char** cur_pos)
{
    ON_DEBUG(printf("In func GetTreeNode\n"));
    ON_DEBUG(printf("\tat the begining we are on sym: %c (%d)\n", **cur_pos, **cur_pos));
    assert( cur_pos && "NULL ptr on buffer ptr, check GetTreeNode func");
    assert(*cur_pos && "NULL ptr on buffer, check SkipSpaces func");

    if(**cur_pos == '(')
    {
        ON_DEBUG(printf("\thave found (\n"));
        *cur_pos = SkipSpaces(++*cur_pos);
        ON_DEBUG(printf("\tnow we are on %c\n", **cur_pos));

        data_t word = {};
        GetAndUnulyzeWord(cur_pos, &word);
        ON_DEBUG(printf("\tafter retunr from analyze we have type: %s(%d)\n", NodeTypes[word.type], word.type));

        ON_DEBUG(printf("\tbefore TreeNodeCtore we are on: %c (%d)\n", **cur_pos, **cur_pos));
        Node_t* left_som  = GetTreeNode(cur_pos);
        Node_t* right_som = GetTreeNode(cur_pos);
        Node_t* node = TreeNodeCtor(&word, left_som, right_som);
        if (!node) return NULL;
        ON_DEBUG(printf("\tafter TreeNodeCtor we have: ptr = %p\n", node));
        ON_DEBUG(printf("\tafter TreeNodeCtor we are on symbol: %c (%d)\n", **cur_pos, **cur_pos));

        if (**cur_pos == ')')
        {
            *cur_pos = SkipSpaces(++*cur_pos);
            return node;
        }
        else
        {
            FREE(node)
            *cur_pos = SkipSpaces(++*cur_pos);
            return NULL;
        }
    }
    else if(strncmp(*cur_pos, "nil", 3) == 0)
    {
        ON_DEBUG(printf("\thave found: nil\n"));

        *cur_pos += sizeof("nil") - 1;
        *cur_pos = SkipSpaces(*cur_pos);

        ON_DEBUG(printf("After skip 'nil' and spaces we are on: %c (%d)\n", **cur_pos, **cur_pos));
        ON_DEBUG(printf("Out from GetTreeNode"));

        return NULL;
    }

    ON_DEBUG(printf(RED_COLOR "\tinvalid word in txt file\n" RESET));

    return NULL;
}
data_t* GetAndUnulyzeWord(char** cur_pos, data_t* data)
{
    assert( cur_pos);
    assert(*cur_pos);
    const size_t START_LEN = 1 << 4;

    char* word = (char*) calloc(START_LEN, sizeof(char));
    assert(word && "memmory allocation err");

    if (!GetWord(&word, START_LEN, cur_pos))
    {
        FREE(word)
        return NULL;
    }
    ON_DEBUG(printf("\twe have read: %s\n", word));

    AnalyzeWord(word, data);
    ON_DEBUG(printf("\tafter analyze we have type: %s(%d)\n", NodeTypes[data->type], data->type));
    FREE(word);
    *cur_pos = SkipSpaces(*cur_pos);
    return data;
}
data_t* AnalyzeWord(char* word, data_t* data)
{
    assert(word);
    double check_num = 0;

    if (sscanf(word, "%lf", &check_num))
    {
        data->type = NUM;
        data->value.num = check_num;
        return data;
    }

    Operator_t check_op = ADD;
    if(FindOperator(word, &check_op))
    {
        data->type = OP;
        data->value.op = check_op;
        return data;
    }

    if(('a' <= word[0] && word[0] <= 'z') || ('A' <= word[0] && word[0] <= 'Z'))
    {
        data->type = VAR;
        data->value.var = strdup(word);
        return data;
    }

    return NULL;
}

//SECTION - write LaTeX
bool FindOperator(char* word, Operator_t* op)
{
    for (size_t i = 0; i < sizeof(Operators)/sizeof(Operators[0]); i++)
    {
        if(strcmp(Operators[i].symbol, word) == 0)
        {
            *op = (Operator_t) i;
            return true;
        }
    }

    return false;
}
void BeginLaTeXDocument(FILE* file)
{
    assert(file);

    fprintf(file,
    "\\documentclass[a4paper,12pt]{article}\n"
    "\\usepackage[T2A]{fontenc}\n"
    "\\usepackage[utf8]{inputenc}\n"
    "\\usepackage{longtable}\n"
    "\\usepackage[left=2cm, right=1.5cm, top=2cm, bottom=2cm]{geometry}\n"
    "\\usepackage{array}\n"
    "\\usepackage{wrapfig}\n"
    "\\usepackage[warn]{mathtext}\n"
    "\\usepackage[russian]{babel}\n"
    "\\usepackage{amssymb}\n"
    "\\usepackage{graphicx, float, multicol, hyperref, pgfplots, amsmath}\n"
    "\\usepackage{pgfplots}\n"
    "\\pgfplotsset{compat=1.18}\n"
    "\\usepackage{tikz}\n"
    "\\usepackage{rotating}\n"
    "\\usepackage{amsmath,amsfonts,amssymb,amsthm,mathtools}\n"
    "\\usepackage{graphicx}\n"
    "\\usepackage{subcaption}\n"
    "\n"
    "\\begin{document}\n"
    "\n"
    "\\begin{titlepage}\n"
    "	\\begin{center}\n"
    "		{\\large МОСКОВСКИЙ ФИЗИКО-ТЕХНИЧЕСКИЙ ИНСТИТУТ (НАЦИОНАЛЬНЫЙ ИССЛЕДОВАТЕЛЬСКИЙ УНИВЕРСИТЕТ)}\n"
    "	\\end{center}\n"
    "	\\begin{center}\n"
    "		{\\large Физтех-школа радиотехники и компьютерных технологий (ФРКТ)}\n"
    "	\\end{center}\n"
    "	\n"
    "	\n"
    "	\\vspace{4.5cm}\n"
    "	{\\huge\n"
    "		\\begin{center}\n"
    "			{\\bf Отчёт о взятии производной произвольной функции}\\\n"
    "		\\end{center}\n"
    "	}\n"
    "	\\vspace{2cm}\n"
    "	\\begin{flushright}\n"
    "		{\\LARGE Автор: \\\\ Ковалев Михаил Андреевич \\\\\n"
    "			\\vspace{0.2cm}\n"
    "			Группа Б01-502}\n"
    "	\\end{flushright}\n"
    "	\\vspace{8cm}\n"
    "	\\begin{center}\n"
    "		г. Долгопрудный\\\\\n"
    "		\\today\n"
    "	\\end{center}\n"
    "\\end{titlepage}\n"
    "\\section{Ваше исходное выражение}\n"
    "\\\\\n"
    );
}
void WriteTreeNodeLaTeX(FILE* file, Node_t* node)
{
    assert(file);
    assert(node);

    switch(node->node_type)
    {
        case NUM:
            if(node->value.num < 0) fprintf(file, "(%g)", node->value.num);
            else                    fprintf(file,  "%g", node->value.num);
            return;
        case VAR:
            fprintf(file, "%s", node->value.var);
            return;
        case OP:
            switch(node->value.op)
            {
                default: return;
                case ADD    : PrintBinaryOperator ("+"     ); break;
                case SUB    : PrintBinaryOperator ("-"     ); break;
                case MUL    : PrintBinaryOperator ("\\cdot"); break;
                case DIV    : PrintDiv            (/*frac*/); break;
                case POW    : PrintPow            (/*^*/   ); break;
                case SQRT   : PrintSqrt           (/*sqrt*/); break;
                case SIN    : PrintUnaryOperator  ("sin"   ); break;
                case COS    : PrintUnaryOperator  ("cos"   ); break;
                case TG     : PrintUnaryOperator  ("tg"    ); break;
                case CTG    : PrintUnaryOperator  ("ctg"   ); break;
                case SH     : PrintUnaryOperator  ("sh"    ); break;
                case CH     : PrintUnaryOperator  ("ch"    ); break;
                case TH     : PrintUnaryOperator  ("th"    ); break;
                case CTH    : PrintUnaryOperator  ("cth"   ); break;
                case ARCSIN : PrintUnaryOperator  ("arcsin"); break;
                case ARCCOS : PrintUnaryOperator  ("arccos"); break;
                case ARCTG  : PrintUnaryOperator  ("arctg" ); break;
                case ARCCTG : PrintUnaryOperator  ("arcctg"); break;
                case LOG    : PrintLog            (/*log*/ ); break;
                case LG     : PrintUnaryOperator  ("lg"    ); break;
                case LN     : PrintUnaryOperator  ("ln"    ); break;
            }
        default:
            return;
    }
    return;
}
void DumpLaTeX(FILE* file, Node_t* node)
{
    fprintf(file, "%s\\\\\n\n\\[", phrases[(long unsigned int)rand() % PHRASES_NUM]);
    WriteTreeNodeLaTeX(file, node);
    fprintf(file, "\\]\n\n");
}
bool NeedBraces(Node_t* node, Node_t* next_node)
{
    assert(node);
    assert(next_node);

    ON_DEBUG(printf("node:      type = %s op = %s\n", NodeTypes[node->node_type], Operators[node->value.op].name));
    ON_DEBUG(printf("next_node: type = %s\n", NodeTypes[next_node->node_type]));
    switch(next_node->node_type)
    {
        case NUM:
        case VAR:
            return false;
        case OP:
            if(Operators[next_node->value.op].priority < Operators[node->value.op].priority)
                return true;
            else
                return false;
        default:
            return false;
    }
    return false;
}
void PrintLog_(FILE* file, Node_t* node)
{
    assert(file);
    assert(node);

    fprintf(file, "\\operatorname{log}_{");
    WriteTreeNodeLaTeX(file, node->right);
    fprintf(file, "}\\left(");
    WriteTreeNodeLaTeX(file, node->left);
    fprintf(file, "\\right) ");
}
void PrintDiv_(FILE* file, Node_t* node)
{
    assert(file);
    assert(node);

    fprintf(file, "\\frac{");
    WriteTreeNodeLaTeX(file, node->left);
    fprintf(file, "}{");
    WriteTreeNodeLaTeX(file, node->right);
    fprintf(file, "}");
}
void PrintPow_(FILE* file, Node_t* node)
{
    assert(file);
    assert(node);
    fprintf(file, "\\left(");
    WriteTreeNodeLaTeX(file, node->left);
    fprintf(file, "\\right)^{");
    WriteTreeNodeLaTeX(file, node->right);
    fprintf(file, "}");
}
void PrintSqrt_(FILE* file, Node_t* node)
{
    assert(file);
    assert(node);

    fprintf(file, "\\sqrt{");
    WriteTreeNodeLaTeX(file, node->left);
    fprintf(file, "} ");
}
void PrintBinaryOperator_(FILE* file, const char* const op, Node_t* node)
{
    assert(file);
    assert(op);
    assert(node);

    bool left_braces_marker = NeedBraces(node, node->left);
    if(left_braces_marker) fprintf(file, "\\left(");
    WriteTreeNodeLaTeX(file, node->left);
    if(left_braces_marker) fprintf(file, "\\right)");

    fprintf(file, "%s ", op);
    bool right_braces_marker = NeedBraces(node, node->right);
    if(right_braces_marker) fprintf(file, "\\left(");
    WriteTreeNodeLaTeX(file, node->right);
    if(right_braces_marker) fprintf(file, "\\right)");
}
void PrintUnaryOperator_(FILE* file, const char* const op, Node_t* node)
{
    assert(file);
    assert(op);
    assert(node);

    fprintf(file, "\\operatorname{%s}\\left(", op);
    WriteTreeNodeLaTeX(file, node->left);
    fprintf(file, "\\right) ");
}
void EndLaTeXDocument(FILE* file)
{
    assert(file);

    fprintf(file,
        "\\end{document}"
    );
}

//SECTION - derivative
Node_t* TakeDerivative(FILE* file, Node_t* node, const char* const var)
{
    assert(var);
    assert(node);
    assert(var);

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
            switch(node->value.op)
            {
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
                case LOG:       new_node = LogDerivative(file, node, var);                                     break;
                case POW:       new_node = PowDerivative(file, node, var);                                     break;
            }
            break;
    }

    #include "UndefDerivativeDSL.h"
    DumpLaTeX(file, new_node);
    return new_node;
}
Node_t* PowDerivative(FILE* file, Node_t* node, const char* const var)
{
    assert(file);
    assert(node);
    assert(var);

    #include "DerivativeDSL.h"
    bool left_flag  = FindVar(node->left,  var);
    bool right_flag = FindVar(node->right, var);

         if(left_flag && !right_flag) return COMP_FUNC(MUL_(c(R), POW_(c(L), SUB_(c(R), n(1)))));  //  n * a^(n - 1) * d(a)
    else if(!left_flag && right_flag) return MUL_(MUL_(POW_(c(L), c(R)), LN_(c(L))), d(R));  //  n^a * ln(n) * d(a)
    else if(left_flag && right_flag)
    {
        Node_t* auxiliary_transformation = POW_(n(M_E), MUL_(LN_(c(L)), c(R)));  // d(e^(ln(a) * b))
        Node_t* new_node = d(auxiliary_transformation);
        DeleteTreeNode(&auxiliary_transformation);
        return new_node;
    }
    else return n(0);  // 0
}
Node_t* LogDerivative(FILE* file, Node_t* node, const char* const var)
{
    assert(file);
    assert(node);
    assert(var);

    #include "DerivativeDSL.h"
    bool left_flag  = FindVar(node->left,  var);
    bool right_flag = FindVar(node->right, var);

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
Node_t* DeepNodeCopy(Node_t* node)
{
    if(node)
    {
        Value_t value = node->value;
        if(node->node_type == VAR)
        {
            value.var = strdup(value.var);
        }
        return TreeNodeCtor_(node->node_type, value, DeepNodeCopy(node->left), DeepNodeCopy(node->right));
    }
    return NULL;
}
bool FindVar(Node_t* node, const char* const var)
{
    assert(node);
    assert(var);

    switch(node->node_type)
    {
        case VAR:
            if (strcmp(node->value.var, var) == 0) return true;
            return false;
        case NUM:
            return false;
        case OP:
            if(node->left)
            {
                if (FindVar(node->left, var)) return true;
            }
            else
            {
                assert(true);
                return false;
            }

            if(node->right)
            {
                if (FindVar(node->right, var)) return true;
            }
            return false;
        default:
            assert(true);
            return false;
    }
}
