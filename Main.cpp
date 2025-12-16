#include "Math_PP.h"

const int TEYLOR_ORDER = 4;
const int MAX_DUMP_DERIVATIVE_ORDER = 3;

int main()
{
    int count_img = 0;
    srand((unsigned int) time(NULL));
    StartHTMLfile();

    Tree_t* user_tree = GetExpression(&count_img);

    FILE* tex_file = fopen("LaTeX.tex","w");
    assert(tex_file);
    BeginLaTeXDocument(tex_file);
    PrintOriginalTree(tex_file, user_tree->root);

    Node_t* exp = user_tree->root;
    Tree_t* derivatives[TEYLOR_ORDER] = {};

    bool is_print = true;
    for(int derivative_order = 0; derivative_order < TEYLOR_ORDER; derivative_order++)
    {
        if(derivative_order > 0) fprintf(tex_file, "Идем дальше...\n\n");
        if(derivative_order < MAX_DUMP_DERIVATIVE_ORDER)
            PrintDerivativeBegining(tex_file);

        Node_t* derivative = TakeDerivative(tex_file, exp, "x", &count_img, &is_print);
        MakePrevNode(derivative);
        derivatives[derivative_order] = TreeCtor(derivative);
        TreeDump(derivative, count_img++);

        if(derivatives[derivative_order]->size < MAX_DUMP_SIZE)
        {
            char* img_name = (char*) calloc(8, sizeof(char));
            sprintf(img_name, "graph%d", derivative_order);
            DrawGraph(tex_file, img_name, user_tree->root, derivatives[derivative_order]->root);
            free(img_name);
        }
        else
        {
            fprintf(tex_file, "\\section*{Построение графика функций}\n\n"
                              "Сказал же, по аналогии. Сами дальше стройте!");
        }
        exp = derivatives[derivative_order]->root;
        is_print = true;
    }

    EndProgram(tex_file, user_tree, derivatives);
    return 0;
}

Tree_t* GetExpression(int* count_img)
{
    assert(count_img);

    char* user_file = ReadFile("Expression.txt");
    assert(user_file && "NULL user_file, check ReadFile func");

    char* cur_pos = user_file;
    Node_t* user_nodes = GetG(&cur_pos);
    MakePrevNode(user_nodes);
    FREE(user_file)

    TreeDump(user_nodes, (*count_img)++);

    return TreeCtor(user_nodes);
}
void EndProgram(FILE* tex_file, Tree_t* user_tree, Tree_t** derivatives)
{
    assert(tex_file);
    assert(user_tree);
    assert(derivatives);

    EndLaTeXDocument(tex_file);
    fclose(tex_file);
    tex_file = NULL;

    EndHTMLfile();

    TreeDtor(&user_tree);
    for(int i = 0; i < TEYLOR_ORDER; i++)
    {
        TreeDtor(&(derivatives[i]));
    }
}
