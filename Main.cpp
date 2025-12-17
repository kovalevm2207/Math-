#include "Math_PP.h"

int main(const int argc, const char* const argv[])
{
    int count_img = 0;
    const char* input_file = NULL;
    srand((unsigned int) time(NULL));
    StartHTMLfile();
    if(argc != 2)
    {
        HelpUser();
        input_file = "Expression.txt";
    }else input_file = argv[1];
    ProgramData_t input_data = GetInputData(input_file);

    fprintf(stderr, "проверка корректности считывания:\n");
    fprintf(stderr, "input_data.taylor_order = %lg\n",   input_data.taylor_order);
    fprintf(stderr, "input_data.expansion_dot = %lg\n",  input_data.expansion_dot);
    fprintf(stderr, "input_data.x.left_border = %lg\n",  input_data.x.left_border);
    fprintf(stderr, "input_data.x.right_border = %lg\n", input_data.x.right_border);
    fprintf(stderr, "input_data.y.left_border = %lg\n",  input_data.y.left_border);
    fprintf(stderr, "input_data.y.right_border = %lg\n", input_data.y.right_border);

    FILE* tex_file = fopen("LaTeX.tex","w");
    assert(tex_file);
    BeginLaTeXDocument(tex_file);
    PrintOriginalTree(tex_file, input_data.user_tree->root);

    Derivative_t* derivatives = GetNDerivatives(tex_file, input_data.user_tree, &count_img);
    Tree_t* taylor_tree = TaylorExpansion(tex_file, derivatives);

    EndProgram(tex_file, derivatives, &input_data);
    TreeDtor(&taylor_tree);
    return 0;
}
void EndProgram(FILE* tex_file, Derivative_t* derivatives, ProgramData_t* input_data)
{
    assert(tex_file);
    assert(derivatives);
    assert(input_data);

    EndLaTeXDocument(tex_file);
    fclose(tex_file);
    tex_file = NULL;

    EndHTMLfile();

    input_data->user_tree = NULL;
    input_data->taylor_order = 0;
    input_data->x.left_border = 0;
    input_data->x.right_border = 0;
    input_data->y.left_border = 0;
    input_data->y.right_border = 0;

    for(int i = 0; i <= TAYLOR_ORDER; i++)
    {
        TreeDtor(&(derivatives[i].tree));
        derivatives[i].data = 0;
    }
    FREE(derivatives)
}
