#include "Math_PP.h"

int main()
{
    int count_img = 0;
    srand((unsigned int) time(NULL));
    StartHTMLfile();

    Tree_t* user_tree = GetExpression("Expression.txt");

    FILE* tex_file = fopen("LaTeX.tex","w");
    assert(tex_file);
    BeginLaTeXDocument(tex_file);
    PrintOriginalTree(tex_file, user_tree->root);

    Derivative_t* derivatives = GetNDerivatives(tex_file, user_tree, &count_img);
    Tree_t* taylor_tree = TaylorExpansion(tex_file, derivatives);

    EndProgram(tex_file, derivatives);
    TreeDtor(&taylor_tree);
    return 0;
}
void EndProgram(FILE* tex_file, Derivative_t* derivatives)
{
    assert(tex_file);
    assert(derivatives);

    EndLaTeXDocument(tex_file);
    fclose(tex_file);
    tex_file = NULL;

    EndHTMLfile();

    for(int i = 0; i <= TAYLOR_ORDER; i++)
    {
        TreeDtor(&(derivatives[i].tree));
        derivatives[i].data = 0;
    }
    FREE(derivatives)
}
