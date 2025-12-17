#include "Teylor.h"

Tree_t* TaylorExpansion(FILE* file, Derivative_t* derivatives)
{
    assert(derivatives);
    assert(file);

    Tree_t* taylor_tree = MakeTaylorTree(derivatives);
    MakeGraphScript(derivatives[0].tree->root, taylor_tree->root, "taylor");

    #define K  derivatives[1].data
    #define Y0 derivatives[0].data
    #define X0 derivatives[0].tree->vars[0].data

    Tree_t* tangent_line = TangentLine(K, Y0 - K*X0, X0, Y0);

    #undef K
    #undef Y0
    #undef X0

    DrawTaylorGraph(file, "Taylor", derivatives[0].tree, taylor_tree->root, tangent_line->root );
    TreeDtor(&tangent_line);

    return taylor_tree;
}
Tree_t* MakeTaylorTree(Derivative_t* derivatives)
{
    assert(derivatives);

    FILE* file = fopen("TaylorExpansion.txt", "w");
    assert(file);

    double factorial = 1;
    fprintf(file, "%f", derivatives[0].data/factorial);
    for(int i = 1; i <= TAYLOR_ORDER; i++)
    {
        factorial *= i;
        fprintf(file, "+%f*(x-%f)^%d", derivatives[i].data/factorial, derivatives[0].tree->vars[0].data, i);
    }
    fprintf(file, "$");
    fclose(file);

    return GetExpression("TaylorExpansion.txt");
}
Tree_t* TangentLine(double k, double b, double x_0, double y_0)
{
    #include "DerivativeDSL.h"
    Node_t* tangent_line = ADD_(MUL_(n(k), v("x")), n(b));
    #include "UndefDerivativeDSL.h"

    FILE* script = fopen("script_plot.gp", "a");
    assert(script);

    fprintf(script, ", ");
    WriteTreeNodeGnuPlot(script, tangent_line);
    fprintf(script,
    " with lines lw 2 lc rgb \"#43A047\"  title \"h(x)\", '-' with points pt 7 ps 0.5 lc \"orange\" title \"A\"\n"
    "%lg %lg\n"
    "e", x_0, y_0);

    fclose(script);
    return TreeCtor(tangent_line);
}
int DrawTaylorGraph(FILE* file, const char* const img_name, Tree_t* base_func, Node_t* taylor_func, Node_t* tangent_line)
{
    assert(file);
    assert(img_name);
    assert(base_func);
    assert(taylor_func);
    assert(tangent_line);

    fprintf(file,
    "\\section{Разложение в ряд тейлора:}\n"
    "Удивительно, но любую функцию в окрестности любой точки, принадлежащей ее области определения, "
    "можно представить в виде суммы степенный функций, что мы с вами и сделаем:\\\\\n\n"
    "subsubsection*{общая формула для разложения в ряд Тейлора в окрестности точки $x_0$ c остаточным членом в форме Пеано:}\n"
    "\\[\\sum_{k=0}^{n}\\frac{f^{(k)}(x_0)}{k!}\\cdot(x-x_0)^k+o((x-x_0)^n)\\]\n\n"
    "Так наша исходная функция:"
    "\\begin{dmath} f(x)=");
    WriteTreeNodeLaTeX(file, base_func->root);
    fprintf(file, "\\end{dmath}\n\n");
    fprintf(file,
    "может быть представлена в виде:");
    fprintf(file, "\\begin{dmath} g(x)=");
    WriteTreeNodeLaTeX(file, taylor_func);
    fprintf(file, "\\end{dmath}\n\nесли мы разложим ее в окрестности $x_0 = %lg$\n", base_func->vars[0].data);
    fprintf(file,
    "Также для проверки разложения первой производной построим касательную в той же точке A($x_0$, $f(x_0)$), к нашей исходной функции. "
    "Уравнение касательной будет выглядеть следующим образом:\n"
    "\\begin{dmath} h(x)=");
    WriteTreeNodeLaTeX(file, tangent_line);
    fprintf(file, "\\end{dmath}\n\n");

    system("gnuplot script_plot.gp");
    fprintf(file, "\\includegraphics[width=1\\textwidth]{Graph/%s.pdf}\n\n", img_name);

    return 0;
}
