#include "PrintFunctions.h"

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
    "\\section{Вывод производной}"
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
                case NOT_OP:
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
    bool need_left = NeedBraces(node, node->left);
    if(need_left) fprintf(file, "\\left(");
    WriteTreeNodeLaTeX(file, node->left);
    if(need_left) fprintf(file, "\\right)");
    fprintf(file, "^{");
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

    fprintf(file, "\\subsubsection*{Попробуем упростить полученное выражение}\n\\\\");
}
void PrintSimplifyRes(FILE* file, Node_t* base_node, Node_t* simple_node, const char* const var, bool flag)
{
    assert(file);
    assert(base_node);
    assert(simple_node);
    assert(var);

    if(flag)
    {
        fprintf(file, "Подтвердил распечатку, печатаю:\n\n");
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
    else fprintf(file, "Что-то я расхотел печатать как-то\n\n");
}
void EndLaTeXDocument(FILE* file)
{
    assert(file);

    fprintf(file,
        "\\end{document}"
    );
}
void WriteTreeNodeGnuPlot(FILE* file, Node_t* node)
{
    assert(file);
    assert(node);

    switch(node->node_type)
    {
        case NUM:
            if(node->value.num < 0) fprintf(file, "(%lg)", node->value.num);
            else                    fprintf(file,  "%lg", node->value.num);
            return;
        case VAR:
            fprintf(file, "%s", node->value.var);
            return;
        case OP:
            switch(node->value.op)
            {
                case NOT_OP:
                default: return;
                case ADD    : PrintPlotBinaryOperator(file, "+",             node); break;
                case SUB    : PrintPlotBinaryOperator(file, "-",             node); break;
                case MUL    : PrintPlotBinaryOperator(file, "*",             node); break;
                case DIV    : PrintPlotBinaryOperator(file, "/",             node); break;
                case POW    : PrintPlotBinaryOperator(file, "**",            node); break;
                case SQRT   : PrintPlotBinaryOperator(file, "sqrt",          node); break;
                case SIN    : PrintPlotUnaryOperator (file, "sin",           node); break;
                case COS    : PrintPlotUnaryOperator (file, "cos",           node); break;
                case TG     : PrintPlotUnaryOperator (file, "tan",           node); break;
                case CTG    : PrintPlotUnaryOperator (file, "1/tan",         node); break;
                case SH     : PrintPlotUnaryOperator (file, "sh",            node); break;
                case CH     : PrintPlotUnaryOperator (file, "ch",            node); break;
                case TH     : PrintPlotUnaryOperator (file, "th",            node); break;
                case CTH    : PrintPlotUnaryOperator (file, "cth",           node); break;
                case ARCSIN : PrintPlotUnaryOperator (file, "asin",          node); break;
                case ARCCOS : PrintPlotUnaryOperator (file, "acos",          node); break;
                case ARCTG  : PrintPlotUnaryOperator (file, "atan" ,         node); break;
                case ARCCTG : PrintPlotUnaryOperator (file, "pi/2.0 - atan", node); break;
                case LOG    : //PrintPlotLogOperator   (file, /*log /log*/     node); break;
                case LG     : PrintPlotUnaryOperator (file, "lg",            node); break;
                case LN     : PrintPlotUnaryOperator (file, "log",            node); break;
            }
        default:
            return;
    }
    return;
}
void PrintPlotBinaryOperator(FILE* file, const char* const op, Node_t* node)
{
    assert(file);
    assert(op);
    assert(node);

    bool left_braces_marker = NeedBraces(node, node->left);
    if(left_braces_marker) fprintf(file, "(");
    WriteTreeNodeGnuPlot(file, node->left);
    if(left_braces_marker) fprintf(file, ")");

    fprintf(file, "%s", op);
    bool right_braces_marker = NeedBraces(node, node->right);
    if(right_braces_marker) fprintf(file, "(");
    WriteTreeNodeGnuPlot(file, node->right);
    if(right_braces_marker) fprintf(file, ")");
}
void PrintPlotUnaryOperator(FILE* file, const char* const op, Node_t* node)
{
    assert(file);
    assert(op);
    assert(node);

    fprintf(file, "%s(", op);
    WriteTreeNodeGnuPlot(file, node->left);
    fprintf(file, ")");
}
