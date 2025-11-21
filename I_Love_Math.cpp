#include "Types.h"

int main()
{
    // reading from file
    char* user_file = ReadFile("Expreshion.txt");
    assert(user_file && "NULL user_file, check ReadFile func");
    char* cur_pos = SkipSpaces(user_file);

    Node_t* user_tree = GetTreeNode(&cur_pos);
    FREE(user_file)
    TreeDump(user_tree, 1);

    // write in LaTeX
    FILE* file = fopen("LaTeX.txt","r");
    assert(file);
    BeginLaTeXDocument(file);
    LaTeXWriteTreeNode();

    DeleteTreeNode(&user_tree);

    return 0;
}


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

    UnulyzeWord(word, data);
    ON_DEBUG(printf("\tafter analyze we have type: %s(%d)\n", NodeTypes[data->type], data->type));
    FREE(word);
    *cur_pos = SkipSpaces(*cur_pos);
    return data;
}


data_t* UnulyzeWord(char* word, data_t* data)
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
    "\\usepackage[utf8]{inputenc}  \n"
    "\\usepackage{longtable}    % длинные таблицы\n"
    "\\usepackage[left=2cm, right=1.5cm, top=2cm, bottom=2cm]{geometry}  % размеры полей\n"
    "\\usepackage{array} \n"
    "\\usepackage{wrapfig}  % обтекание текстом\n"
    "\\usepackage[warn]{mathtext}  % русские буквы в математических формулах\n"
    "\\usepackage[russian]{babel}  % русские индексы \n"
    "\\usepackage{ amssymb }\n"
    "\\usepackage{graphicx, float, multicol, hyperref, pgfplots, amsmath}\n"
    "\\usepackage{pgfplots}\n"
    "\\pgfplotsset{compat=1.18}\n"
    "\\usepackage{tikz}\n"
    "\\usepackage{rotating}  % для sidewaysfigure\n"
    "\\usepackage[english,russian]{babel}  \n"
    "\\usepackage{amsmath,amsfonts,amssymb,amsthm,mathtools}  % математические штуки\n"
    "\\usepackage{graphicx}  % картинки\n"
    "\\usepackage{subcaption} % картинки в ряд\n"
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
    "	\vspace{4.5cm}\n"
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
    );
}
