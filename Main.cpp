#include "Math_PP.h"

const int TEYLOR_ORDER = 2;

int main()
{
    int count_img = 0;

    //SECTION - reading from file
        char* user_file = ReadFile("Expression.txt");
        assert(user_file && "NULL user_file, check ReadFile func");
        char* cur_pos = SkipSpaces(user_file);

        Node_t* user_nodes = GetG(&cur_pos);
        MakePrevNode(user_nodes);
        FREE(user_file)

        StartHTMLfile();
        TreeDump(user_nodes, count_img++);

    //SECTION - user_tree struct
        Tree_t* user_tree = TreeCtor(user_nodes);

    //SECTION - write in LaTeX
        srand((unsigned int) time(NULL));

        FILE* tex_file = fopen("LaTeX.tex","w");
        assert(tex_file);
        BeginLaTeXDocument(tex_file);
        PrintOriginalTree(tex_file, user_nodes);

        Node_t* user_nodes_copy = DeepNodeCopy(user_nodes);
        Tree_t* user_tree_copy = TreeCtor(user_nodes_copy);
        SimplifyExpression(tex_file, user_tree_copy, user_tree, &count_img);
        TreeDtor(&user_tree_copy);

    //SECTION - calc user_tree:
        PrintCalcBegining(tex_file);
        PrintCalcResult(tex_file, user_tree);

    Node_t* exp = user_tree->root;
    Tree_t* derivatives[TEYLOR_ORDER] = {};
    for(int i = 0; i < TEYLOR_ORDER; i++)
    {
        PrintDerivativeBegining(tex_file);
        Node_t* derivative = TakeDerivative(tex_file, exp, "x");
        MakePrevNode(derivative);
        TreeDump(derivative, count_img++);

        derivatives[i] = TreeCtor(derivative);

        SimplifyExpression(tex_file, user_tree, derivatives[i], &count_img);
        char* img_name = (char*) calloc(8, sizeof(char));
        sprintf(img_name, "graph%d", i);

        DrawGraph(tex_file, img_name, user_tree->root, derivatives[i]->root);
        exp = derivatives[i]->root;
        free(img_name);
    }

    //SECTION - end of the program
        EndLaTeXDocument(tex_file);
        fclose(tex_file);
        tex_file = NULL;

        EndHTMLfile();

        TreeDtor(&user_tree);
        for(int i = 0; i < TEYLOR_ORDER; i++)
            TreeDtor(&(derivatives[i]));

    return 0;
}
