#include "Graph.h"

int DrawGraph(FILE* file, const char* const img_name, Node_t* first_func, Node_t* sec_func)
{
    assert(first_func);
    fprintf(file,
    "\\section{Построение графиков функций:}\n"
    "Построим график функции :\\\\\n\n"
    "\\begin{dmath} f(x)=");
    WriteTreeNodeLaTeX(file, first_func);
    fprintf(file, "\\end{dmath}\n\n");
    if(sec_func)
    {
        fprintf(file, "\\begin{dmath} g(x)=");
        WriteTreeNodeLaTeX(file, sec_func);
        fprintf(file, "\\end{dmath}\n\n");
    }
    MakeGraphScript(first_func, sec_func, img_name);

    system("gnuplot script_plot1.gp");
    fprintf(file, "\\includegraphics[width=1\\textwidth]{Graph/%s.pdf}\n\n", img_name);

    return 0;
}
int MakeGraphScript(Node_t* first_func, Node_t* sec_func, const char* const img_name)
{
    assert(first_func); assert(img_name);

    FILE* script = fopen("script_plot1.gp", "w");
    assert(script);

    fprintf(script,
    "set terminal pdfcairo enhanced color size 18cm,9cm font \"Arial,12\"\n"
    "set output \"Graph/%s.pdf\"\n"
    "set xrange [-0.5:0.5]\n"
    "set yrange [-15:15]\n"
    "set samples 10000\n"
    "set xtics 1\n"
    "set ytics 1\n"
    "set mxtics 10\n"
    "set mytics 10\n"
    "set arrow from graph 0, first 0 to graph 1, first 0 nohead lw 2 lc \"black\" front\n"
    "set arrow from first 0, graph 0 to first 0, graph 1 nohead lw 2 lc \"black\" front\n"
    "set grid xtics ytics mxtics mytics lt 1 lc rgb \"#777a7b\" lw 1, lt 0 lc rgb \"#777a7b\" lw 0.5\n"
    "set key right top\n"
    "set arrow from 9.5, 0 to 10, 0 head size 0.08,20 lw 2 lc \"black\" front\n"
    "set arrow from 0, 4.5 to 0, 5 head size 0.08,20 lw 2 lc \"black\" front\n", img_name);

    fprintf(script, "plot ");
    WriteTreeNodeGnuPlot(script, first_func);
    fprintf(script,  " with lines lw 2 lc rgb \"%s\" title \"f(x)\"", GenerateColor(first_func));
    if(sec_func)
    {
        fprintf(script, ", ");
        WriteTreeNodeGnuPlot(script, sec_func);
        fprintf(script,  " with lines lw 2 lc rgb \"%s\"  title \"g(x)\"", GenerateColor(sec_func));
    }
    fprintf(script, "\n");

    fclose(script);
    return 0;
}
