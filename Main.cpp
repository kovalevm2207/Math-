#include "Math_PP.h"

int main()
{
    int count_img = 0;
    srand((unsigned int) time(NULL));
    StartHTMLfile();

    Tree_t* user_tree = GetExpression(&count_img, "Expression.txt");

    FILE* tex_file = fopen("LaTeX.tex","w");
    assert(tex_file);
    BeginLaTeXDocument(tex_file);
    PrintOriginalTree(tex_file, user_tree->root);

    Derivative_t* derivatives = GetNDerivatives(tex_file, user_tree, &count_img);

    EndProgram(tex_file, derivatives);
    return 0;
}

Tree_t* GetExpression(int* count_img, const char* const file_name)
{
    assert(count_img);
    assert(file_name);

    char* user_file = ReadFile(file_name);
    assert(user_file && "NULL user_file, check ReadFile func");

    char* cur_pos = user_file;
    Node_t* user_nodes = GetG(&cur_pos);
    MakePrevNode(user_nodes);
    FREE(user_file)

    TreeDump(user_nodes, (*count_img)++);

    return TreeCtor(user_nodes);
}
void EndProgram(FILE* tex_file, Derivative_t* derivatives)
{
    assert(tex_file);
    assert(derivatives);

    EndLaTeXDocument(tex_file);
    fclose(tex_file);
    tex_file = NULL;

    EndHTMLfile();

    for(int i = 0; i <= TEYLOR_ORDER; i++)
    {
        TreeDtor(&(derivatives[i].tree));
        derivatives[i].data = 0;
    }
    FREE(derivatives)
}
