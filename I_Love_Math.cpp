#include "Types.h"

int main()
{
    //SECTION - reading from file
        char* user_file = ReadFile("Expression.txt");
        assert(user_file && "NULL user_file, check ReadFile func");
        char* cur_pos = SkipSpaces(user_file);

        Node_t* user_nodes = GetTreeNode(&cur_pos);
        MakePrevNode(user_nodes);
        FREE(user_file)

        StartHTMLfile();
        TreeDump(user_nodes, 1);

    //SECTION - user_tree struct
        Tree_t* user_tree = TreeCtor(user_nodes);

    //SECTION - write in LaTeX
        srand((unsigned int) time(NULL));

        FILE* tex_file = fopen("LaTeX.tex","w");
        assert(tex_file);
        BeginLaTeXDocument(tex_file);
        PrintOriginalTree(tex_file, user_nodes);

    //SECTION - calc user_tree:
        PrintCalcBegining(tex_file);
        PrintCalcResult(tex_file, user_tree);

    //SECTION - derivative
        PrintDerivativeBegining(tex_file);
        Node_t* first_derivative = TakeDerivative(tex_file, user_nodes, "x");
        MakePrevNode(first_derivative);
        TreeDump(first_derivative, 3);

        Tree_t* first_derivative_tree = TreeCtor(first_derivative);

    //SECTION - simplify first_derivative_tree:
        PrintBeginSimplify(tex_file);
        ConstantFolding(first_derivative);
        TreeDump(first_derivative, 4);
        PrintConstantFolding(tex_file, user_nodes, first_derivative, "x");
        first_derivative = NeutralElementElimination(first_derivative);
        TreeDump(first_derivative, 5);
        PrintConstantFolding(tex_file, user_nodes, first_derivative, "x");

    //SECTION - end of the program
        EndLaTeXDocument(tex_file);
        fclose(tex_file);
        tex_file = NULL;

        EndHTMLfile();

        TreeDtor(&user_tree);
        TreeDtor(&first_derivative_tree);
        DeleteTreeNode(&first_derivative);
        DeleteTreeNode(&user_nodes);

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
TreeErr_t MakePrevNode(Node_t* node)
{
    if(!node) return TREE_OK;
    if(node->left)
    {
        node->left->prev_node = &node->left;
        MakePrevNode(node->left);
    }
    if(node->right)
    {
        node->right->prev_node = &node->right;
        MakePrevNode(node->right);
    }

    return TREE_OK;
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
    "\\usepackage[normalem]{ulem}\n"
    "\\usepackage{breqn}\n"
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
    "\n"
    "\n"
    "	\\vspace{4.5cm}\n"
    "	{\\huge\n"
    "		\\begin{center}\n"
    "			{\\bf Отчёт о взятии производной произвольной функции}\\\\\n"
    "			\\large {Или как я перестал бояться \\sout{и полюбил} дифференцирование}\n"
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
    "\n"
    "\\section*{Введение: \\\\Немного о производных для тех, кто в танке (или на паре)}\n"
    "\\addcontentsline{toc}{section}{Введение}\n"
    "\n"
    "\\subsection*{Что это вообще такое?}\n"
    "\n"
    "Если вы читаете этот отчёт, значит, вам тоже не удалось отмазаться от домашнего задания по матану. \\\\\\textbf{Производная} — это такая магическая палочка для функций, которая показывает, как быстро они растут или падают.\n"
    "\n"
    "Представьте, что вы едете на самокате. Ваша координата — это функция от времени $S(t)$. Ваша скорость в какой-то момент — это и есть производная $S'(t)$. Резко затормозили? Поздравляю, вы только что почувствовали на себе вторую производную (ускорение) и, возможно, перелетели через руль.\n"
    "\n"
    "\\textbf{Формально} (это слово заставляет звучать умнее), производная функции $f(x)$ в точке $x_0$ определяется как предел:\n"
    "\n"
    "\\[\n"
    "f'(x_0) = \\lim_{\\Delta x \\to 0} \\frac{f(x_0 + \\Delta x) - f(x_0)}{\\Delta x}\n"
    "\\]\n"
    "\n"
    "Если вы поняли это с первого раза, возможно, вы гений. Если нет — добро пожаловать в клуб! Все мы просто запоминаем, как брать производные от разных функций, и делаем вид, что понимаем, что такое предел. Главное — делать умное лицо и кивать, когда преподаватель говорит \"дельта икс стремится к нулю\".\n"
    "\n"
    "\\subsection*{Зачем это нужно?}\n"
    "\n"
    "Помимо того, что это отличный способ потратить три часа субботнего вечера, производные используются:\n"
    "\\begin{itemize}\n"
    "    \\item \\textbf{В физике:} чтобы найти скорость, ускорение и заодно оправдать, почему вы не смогли поймать мяч (\"Я же рассчитывал траекторию через производную!\").\n"
    "    \\item \\textbf{В экономике:} чтобы понять, когда пора прекращать производить очередную партию кринжовых чехлов для телефона.\n"
    "    \\item \\textbf{В жизни:} чтобы блеснуть умом на вечеринке (не рекомендуется, иначе придется объяснять, почему вас больше не зовут на вечеринки).\n"
    "    \\item \\textbf{В сессию:} чтобы понять, с какой скоростью растёт паника по мере приближения дедлайна (производная паники по времени — очень интересная функция!).\n"
    "\\end{itemize}\n"
    "\n"
    "\\subsection*{Памятка юного дифференцировщика (Шпаргалка по производных)}\n"
    "\n"
    "\\textbf{Правила дифференцирования:}\n"
    "\n"
    "\\begin{tabular}{p{0.45\\textwidth}p{0.45\\textwidth}}\n"
    "    $(u + v)' = u' + v'$ & $(u - v)' = u' - v'$ \\\\\n"
    "    $(u \\cdot v)' = u' \\cdot v + u \\cdot v'$ & $\\left(\\dfrac{u}{v}\\right)' = \\dfrac{u' \\cdot v - u \\cdot v'}{v^2}$ \\\\\n"
    "    $(f(g(x)))' = f'(g(x)) \\cdot g'(x)$ & \\\\\n"
    "\\end{tabular}\n"
    "\n"
    "\\vspace{5mm}\n"
    "\n"
    "\\textbf{Производные классических функций:}\n"
    "\n"
    "\\begin{tabular}{p{0.45\\textwidth}p{0.45\\textwidth}}\n"
    "    $(x^n)' = n \\cdot x^{n-1}$ & $(e^x)' = e^x$ \\\\\n"
    "    $(a^x)' = a^x \\cdot \\ln a$ & $(\\ln x)' = \\dfrac{1}{x}$ \\\\\n"
    "    $(\\log_a x)' = \\dfrac{1}{x \\cdot \\ln a}$ & $(\\lg x)' = \\dfrac{1}{x \\cdot \\ln 10}$ \\\\\n"
    "    $(\\sin x)' = \\cos x$ & $(\\cos x)' = -\\sin x$ \\\\\n"
    "    $(\\tan x)' = \\dfrac{1}{\\cos^2 x}$ & $(\\cot x)' = -\\dfrac{1}{\\sin^2 x}$ \\\\\n"
    "    $(\\arcsin x)' = \\dfrac{1}{\\sqrt{1 - x^2}}$ & $(\\arccos x)' = -\\dfrac{1}{\\sqrt{1 - x^2}}$ \\\\\n"
    "    $(\\arctan x)' = \\dfrac{1}{1 + x^2}$ & $(\\text{arccot } x)' = -\\dfrac{1}{1 + x^2}$ \\\\\n"
    "    $(\\sinh x)' = \\cosh x$ & $(\\cosh x)' = \\sinh x$ \\\\\n"
    "    $(\\tanh x)' = \\dfrac{1}{\\cosh^2 x}$ & $(\\coth x)' = -\\dfrac{1}{\\sinh^2 x}$ \\\\\n"
    "    $(\\sqrt{x})' = \\dfrac{1}{2\\sqrt{x}}$ & \\\\\n"
    "\\end{tabular}\n"
    "\n"
    "\\subsection*{О чём этот отчёт?}\n"
    "\n"
    "В этом эпическом труде будет подробно, с матами (математическими, конечно же), разобрано взятие производной от конкретной функции. Мы пройдём весь путь от невинного взгляда на условие до триумфального финального ответа, обходя все подводные камни вроде забытых минусов и неправильно применённого цепного правила.\n"
    "\n"
    "Цель: выжить и найти $y'$. Поехали! И помните: если вы не сделали ни одной ошибки в знаке — вы, вероятно, спите и это всего лишь сон.\n");
}
void PrintOriginalTree(FILE* file, Node_t* root)
{
    fprintf(file,
    "\\section*{Исходник:}\n"
    "\n"
    "Не смотря на все ваши попытки ввести самое сложное, на ваш взгляд уравнение, которое обязательно сломает здесь все, \n"
    "мне все таки удалось (ну точнее просто пришлось на самом деле) считать ваше выражение, и собственно оно выглядит вот так:\n"
    "\\begin{dmath}");
    WriteTreeNodeLaTeX(file, root);
    fprintf(file,
    "\\end{dmath}\n"
    "(Знаете, мне нет разницы с чем я буду сейчас работать, но вы то хоть сами понимаете, что написали?)\n\n");
}
void PrintCalcBegining(FILE* file)
{
    fprintf(file,
    "\\section{Вычисление исходного выражения}\n"
    "Наверняка вам было бы интересно узнать (вы же у нас тут все такие любопытные), чему будет равняться ваше выражение, которое даже Пе*****чу "
    "либо вообще никогда в жизни не встречалось, либо снится в ночных кошмарах:\\\n"
    "приступим:\n");
}
void PrintCalcResult(FILE* file, Tree_t* tree)
{
    if(tree->vars_num != 0)
    {
        GetVarsValues(tree);
        fprintf(file,
        "\\begin{align*}\n\t");
        for (size_t i = 0; i < tree->vars_num; i++)
        {
            fprintf(file, "%s = %lg,", tree->vars[i].name, tree->vars[i].data);
            if(((i + 1) % 3) == 0) fprintf(file, "\\\\\n\t");
            else                   fprintf(file, "\t&&\t");
        }
        fprintf(file,
        "\\end{align*}\n\n"
        "\n");
    }
    fprintf(file,
    "%s\n"
    "\n"
    "\\begin{dmath}", TakeDerivativePhrases[(long unsigned int)rand() % TAKE_DERIVATIVE_PHRASES_NUM]);
    WriteTreeNodeLaTeX(file, tree->root);
    fprintf(file,
    "=%lg\\end{dmath}\n"
    "\n", CalcTreeNode(tree->root, tree->vars, tree->vars_num));
}
void PrintDerivativeBegining(FILE* file)
{
    assert(file);

    fprintf(file,
    "\\section{Вывод формулы первой производной}"
    "Настала пора какой-то магии вне Хогвартса, потому что я просто не знаю , как из нашего такого маленького красивого уравнения мог получится такой страшный монстр...\\\n\n"
    "Тем не менее нам предстоит все это пронаблюдать, только не увлекайтесь и не вздумайте разбираться в том, что здесь происходит,\n"
    "а то рискуете в будущем попасть на кафедру вышмата...\n\n"
    "\\subsection*{Прямой расчет по правилам, без упрощения:}\\\\\n"
    "Давайте последовательно разберем каждый шаг:\n\n");
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
    assert(file);
    if(!node)
    {
        fprintf(file, "На данном этапе свертку констант произвести невозможно\\\\\n");
        return;
    }
    fprintf(file, "%s\\\\\n"
    "\n"
    "\\begin{dmath}", TakeDerivativePhrases[(long unsigned int)rand() % TAKE_DERIVATIVE_PHRASES_NUM]);
    WriteTreeNodeLaTeX(file, node);
    fprintf(file, "\\end{dmath}\n\n");
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
void PrintBeginSimplify(FILE* file)
{
    assert(file);

    fprintf(file, "\\subsection*{Попробуем упростить полученное выражение}\n\\\\");
}
void PrintConstantFolding(FILE* file, Node_t* base_node, Node_t* simple_node, const char* const var)
{
    assert(file);
    assert(base_node);
    assert(simple_node);
    assert(var);

    fprintf(file,
    "%s\\\\\n", ConstantFoldingPhrases[(long unsigned int)rand() % CONST_FOLDING_PHRASES_NUM]);
    fprintf(file,
    "\\begin{dmath}\n\t"
    "\\frac{d}{d%s}\\left(", var);
    WriteTreeNodeLaTeX(file, base_node);
    fprintf(file,
    "\\right) = ");
    WriteTreeNodeLaTeX(file, simple_node);
    fprintf(file,
    "\\end{dmath}\n\n");

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
    fprintf(file, "%s\n", TakeDerivativePhrases[(long unsigned int)rand() % TAKE_DERIVATIVE_PHRASES_NUM]);
    fprintf(file, "\\begin{dmath}"
                  "\\frac{d}{d%s}\\left(", var);
    WriteTreeNodeLaTeX(file, node);
    fprintf(file, "\\right)=");
    WriteTreeNodeLaTeX(file, new_node);
    fprintf(file, "\\end{dmath}\n");
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

//SECTION - calc
double CalcTree(Tree_t* tree)
{
    assert(tree);

    GetVarsValues(tree);
    TreeStructDump(tree);

    return CalcTreeNode(tree->root, tree->vars, tree->vars_num);
}
TreeErr_t GetVarsValues(Tree_t* tree)
{
    assert(tree);

    for(size_t i = 0; i < tree->vars_num; i++)
    {
        printf("Введите значение переменной %s:\n\n\t\t", tree->vars[i].name);
        scanf("%lg", &tree->vars[i].data);
        printf("\n\n");
    }

    return TREE_OK;
}
double CalcTreeNode(const Node_t* node, const Var_t* vars, size_t vars_num)
{
    assert(node);
    assert(vars);

    switch(node->node_type)
    {
        default:
            assert(true);
            return 0;
        case NUM:
            return node->value.num;
        case VAR:
            return vars[FindVarPos(node->value.var, vars, vars_num)].data;
        case OP:
            double result = 0;

            #define LC CalcTreeNode(node->left,  vars, vars_num)
            #define RC CalcTreeNode(node->right, vars, vars_num)

            switch(node->value.op)
            {
                default:
                    assert(true);
                    return 0;
                case ADD:       result = LC + RC;           break;
                case SUB:       result = LC - RC;           break;
                case MUL:       result = LC * RC;           break;
                case DIV:       result = LC / RC;           break;
                case SQRT:      result = sqrt(LC);          break;
                case SIN:       result = sin(LC);           break;
                case COS:       result = cos(LC);           break;
                case TG:        result = tan(LC);           break;
                case CTG:       result = 1/tan(LC);         break;
                case SH:        result = sinh(LC);          break;
                case CH:        result = cosh(LC);          break;
                case TH:        result = tanh(LC);          break;
                case CTH:       result = 1/tanh(LC);        break;
                case ARCSIN:    result = asin(LC);          break;
                case ARCCOS:    result = acos(LC);          break;
                case ARCTG:     result = atan(LC);          break;
                case ARCCTG:    result = M_PI_2 - atan(LC); break;
                case LG:        result = log10(LC);         break;
                case LN:        result = log(LC);           break;
                case LOG:       result = log(LC)/log(RC);   break;
                case POW:       result = pow(LC, RC);       break;
            }

            #undef LC
            #undef RC

            return result;

            break;
    }

    assert(true);
    return 0;
}
size_t FindVarPos(const char* const name, const Var_t* const vars, size_t vars_num)
{
    assert(name);
    assert(vars);

    for(size_t pos = 0; pos < vars_num; pos++)
    {
        if(strcmp(name, vars[pos].name) == 0) return pos;
    }

    assert(true);
    return 0;
}


//SECTION - simplify expression
TreeErr_t SimplifyExpression(Tree_t* tree)
{
    assert(tree);

    size_t new_size = 0;

    do
    {
        new_size = tree->size;
        tree->root = ConstantFolding(tree->root);
        NeutralElementElimination(tree->root);
        tree->depth = GetTreeDepth(tree->root);
        tree->size = CountTreeSize(tree->root);
    }
    while(tree->size != new_size);

    return TREE_OK;
}
Node_t* ConstantFolding(Node_t* node)
{
    if(!node) return NULL;

    Node_t* new_node = NULL;

    switch(node->node_type)
    {
        default:
            assert(true);
            return NULL;
        case VAR: return NULL;
        case NUM: return node;
        case OP:
        {
            Node_t* new_left  = ConstantFolding(node->left);
            Node_t* new_right = ConstantFolding(node->right);

            switch(node->value.op)
            {
                default: assert(true); return NULL;
                case ADD: case SUB: case MUL:
                case DIV: case POW: case LOG:
                {
                    new_node = BinaryConstantFolding(node, new_left, new_right);
                    break;
                }
                case TG:     case SH:     case CH:
                case TH:     case LG:     case LN:
                case SIN:    case COS:    case CTG:
                case CTH:    case SQRT:   case ARCTG:
                case ARCSIN: case ARCCOS: case ARCCTG:
                {
                    new_node = UnaryConstantFolding(node, new_left);
                    break;
                }
            }
            return new_node;
        }
    }
}
Node_t* UnaryConstantFolding(Node_t* node, Node_t* new_left)
{
    assert(node);

    #define LS new_left->value.num
    #define RS new_right->value.num

    Node_t* new_node = NULL;

    if(new_left)
    {
        switch(node->value.op)
        {
            case SQRT:   new_node = TreeNodeCtor_(NUM, {.num = sqrt(LS)       }, NULL, NULL); break;
            case SIN:    new_node = TreeNodeCtor_(NUM, {.num = sin(LS)        }, NULL, NULL); break;
            case COS:    new_node = TreeNodeCtor_(NUM, {.num = cos(LS)        }, NULL, NULL); break;
            case TG:     new_node = TreeNodeCtor_(NUM, {.num = tan(LS)        }, NULL, NULL); break;
            case CTG:    new_node = TreeNodeCtor_(NUM, {.num = 1/tan(LS)      }, NULL, NULL); break;
            case SH:     new_node = TreeNodeCtor_(NUM, {.num = sinh(LS)       }, NULL, NULL); break;
            case CH:     new_node = TreeNodeCtor_(NUM, {.num = cosh(LS)       }, NULL, NULL); break;
            case TH:     new_node = TreeNodeCtor_(NUM, {.num = tanh(LS)       }, NULL, NULL); break;
            case CTH:    new_node = TreeNodeCtor_(NUM, {.num = 1/tanh(LS)     }, NULL, NULL); break;
            case ARCSIN: new_node = TreeNodeCtor_(NUM, {.num = asin(LS)       }, NULL, NULL); break;
            case ARCCOS: new_node = TreeNodeCtor_(NUM, {.num = acos(LS)       }, NULL, NULL); break;
            case ARCTG:  new_node = TreeNodeCtor_(NUM, {.num = atan(LS)       }, NULL, NULL); break;
            case ARCCTG: new_node = TreeNodeCtor_(NUM, {.num = M_PI_2-atan(LS)}, NULL, NULL); break;
            case LG:     new_node = TreeNodeCtor_(NUM, {.num = log10(LS)      }, NULL, NULL); break;
            case LN:     new_node = TreeNodeCtor_(NUM, {.num = log(LS)        }, NULL, NULL); break;
            case ADD: case SUB: case MUL:
            case DIV: case POW: case LOG:
            default: assert(true); return NULL;
        }
        if(node->prev_node) *(node->prev_node) = new_node;
        DeleteTreeNode(&node);
    }

    #undef LS
    #undef RS

    return new_node;
}
Node_t* BinaryConstantFolding(Node_t* node, Node_t* new_left, Node_t* new_right)
{
    assert(node);

    #define LS new_left->value.num
    #define RS new_right->value.num

    Node_t* new_node = NULL;

    if(new_left && new_right)
    {
        switch(node->value.op)
        {
            case ADD: new_node = TreeNodeCtor_(NUM, {.num = LS + RS        }, NULL, NULL); break;
            case SUB: new_node = TreeNodeCtor_(NUM, {.num = LS - RS        }, NULL, NULL); break;
            case MUL: new_node = TreeNodeCtor_(NUM, {.num = LS * RS        }, NULL, NULL); break;
            case DIV: new_node = TreeNodeCtor_(NUM, {.num = LS / RS        }, NULL, NULL); break;
            case POW: new_node = TreeNodeCtor_(NUM, {.num = pow(LS,RS)     }, NULL, NULL); break;
            case LOG: new_node = TreeNodeCtor_(NUM, {.num = log(LS)/log(RS)}, NULL, NULL); break;
            case TG:     case SH:     case CH:
            case TH:     case LG:     case LN:
            case SIN:    case COS:    case CTG:
            case CTH:    case SQRT:   case ARCTG:
            case ARCSIN: case ARCCOS: case ARCCTG:
            default: assert(true); return NULL;
        }
        if(node->prev_node) *(node->prev_node) = new_node;
        DeleteTreeNode(&node);
    }

    #undef LS
    #undef RS

    return new_node;
}
Node_t* NeutralElementElimination(Node_t* node)
{
    assert(node);

    switch(node->node_type)
    {
        default: assert(true); return NULL;
        case NUM:
        case VAR:
            return NULL;
        case OP:
        {
            switch(node->value.op)
            {
                default: assert(true); return NULL;
                case ADD: // simplify --> (expression) + 0 = (expression) or  0 + (expression) = (expression)
                {
                    return SimplifyTerms(&node, node->left, node->right) ?: SimplifyTerms(&node, node->right, node->left);
                }
                case SUB: // simplify --> (expression) - 0 = (expression) or  0 - (expression) = (expression)
                case MUL:
                case DIV:    case POW:    case LOG:
                case TG:     case SH:     case CH:
                case TH:     case LG:     case LN:
                case SIN:    case COS:    case CTG:
                case CTH:    case SQRT:   case ARCTG:
                case ARCSIN: case ARCCOS: case ARCCTG:
                return NULL;
            }
        }
    }
}
Node_t* SimplifyTerms(Node_t** node, Node_t* simple_node, Node_t* complex_node)
{
    assert(node);
    assert(simple_node);
    assert(complex_node);

    switch(simple_node->node_type)
    {
        case NUM:
        {
            Node_t* complex_node_copy = NULL;
            if(DoubleCompare(simple_node->value.num, 0) == 0)
            {
                complex_node_copy = DeepNodeCopy(complex_node);
                if((*node)->prev_node) *((*node)->prev_node) = complex_node_copy;
                MakePrevNode(complex_node_copy);
                DeleteTreeNode(node);
            }
            return complex_node_copy;
        }
        case OP:
        {
            Node_t* simple_result = NeutralElementElimination(simple_node);
            Node_t* simple_result_copy = NULL;
            if(simple_result)
            {
                simple_result_copy = DeepNodeCopy(simple_result);
                if((*node)->prev_node) *((*node)->prev_node) = simple_result_copy;
                MakePrevNode(simple_result_copy);
                DeleteTreeNode(node);
            }
            return simple_result_copy;
        }
        case VAR:              return NULL;
        default: assert(true); return NULL;
    }
}
int DoubleCompare(double a, double b)
{
    if(fabs(a - b) < PRECISION) return 0;
    return a < b ? -1 : 1;
}
