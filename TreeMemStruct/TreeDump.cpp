#include "TreeDump.h"


FILE* StartHTMLfile(void)
{
    FILE* dump_file = fopen("dump.html", "w");
    if(dump_file == NULL) return NULL;

    time_t now = time(NULL);
    struct tm *time_info = localtime(&now);

    fprintf(dump_file, "<!DOCTYPE html>\n"
                       "<html>\n"
                       "<head>\n"
                       "    <title>Tree Dump</title>\n"
                       "</head>\n"
                       "<body>\n"
                       "<style>\n"
                       "    * {\n"
                       "       font-size: 20px;\n"
                       "    }\n"
                       "</style>\n"
                       "<pre> Creation time (hours:min): %02d:%02d\n"
                            " data (day:month:year): %02d:%02d:%d </pre>",
                        time_info->tm_hour, time_info->tm_min,
                        time_info->tm_mday, time_info->tm_mon + 1,
                        time_info->tm_year + 1900);
    fclose(dump_file);
    return  dump_file;
}


TreeErr_t CreateDotFile(const Node_t* node)
{
    assert(node != NULL);

    FILE* dump_file = fopen("TreeMemStruct/svg_dot/dump.dot", "w");
    assert(dump_file != NULL);

    fprintf(dump_file, "digraph DUMP\n"
                       "{\n"
                       "    rankdir=TB;\n"
                       "    splines=false;\n"
                       "    node["
                                // "shape=hexagon,"
                                // "style=\"filled\","
                                // "fillcolor=\"red\","
                                // "fontcolor=\"white\","
                                "fontname=\"Arial\","
                                "fontsize=24,"
                                // "width=1.8,"
                                // "height=1.8"
                                "];\n"
                       "    edge [style=\"bold\", "
                                 "arrowhead=\"normal\", "
                                 "splines=line, "
                                 "dir=normal]\n");

    int node_count = 0;
    MakeNodes(node, &node_count, dump_file);

    node_count = 0;
    MakeArrows(node, &node_count, dump_file);

    fprintf(dump_file,"}\n");

    fclose(dump_file);
    return TREE_OK;
}


void MakeNodes(const Node_t* node, int* node_count, FILE* file)
{
    assert(node != NULL);
    assert(file != NULL);

    fprintf(file,
        "    node%d [shape=plaintext, label=<\n"
        "        <TABLE BORDER=\"0\" "         // толщина внешней рамки таблицы
                 "CELLBORDER=\"2\" "           // толщина рамок ячеек
                 "CELLSPACING=\"3\" "          // расстояние между ячейками
                 "CELLPADDING=\"3\" "          // отступы внутри ячеек (от текста до рамки)
                 "BGCOLOR=\"#0bdfeeff\" "    // Цвет фона всей таблицы
                 "FIXEDSIZE=\"FALSE\" "        // фиксированный размер (игнорирует содержимое)
                 "ALIGN=\"CENTER\" "           // Горизонтальное выравнивание
                 "VALIGN=\"MIDDLE\" "          // Вертикальное выравнивание
                 "SIDES=\"LRTB\" "             // Какие стороны рамки показывать
                 ">\n"
        "            <TR><TD PORT=\"p\" "
                        "BGCOLOR=\"%s\" "
                        "COLSPAN=\"2\" "
                        "COLOR=\"#000000\">"
                            "%p"
                    "</TD></TR>\n"
        "            <TR><TD PORT=\"t\" "
                        "BGCOLOR=\"#B0E0B0ff\" "
                        "COLSPAN=\"2\" "
                        "COLOR=\"#000000ff\">"
                            "type = %s (enum = %d)"
                    "</TD></TR>\n"
        "            <TR><TD PORT=\"v\" "
                        "BGCOLOR=\"#c5a1e6ff\" "
                        "COLSPAN=\"2\" "
                        "COLOR=\"#000000\">"
                            "value = ",
        *node_count, GenerateColor(node), node, NodeTypes[node->node_type], node->node_type
    );
    switch(node->node_type)
    {
        case NUM:
            fprintf(file, "%g ", node->value.num);
            break;
        case VAR:
            fprintf(file, "\"%s\" ", node->value.var);
            break;
        case OP:
            fprintf(file, "\"%s\" or \"%s\" or %d ", Operators[node->value.op].name, Operators[node->value.op].symbol, node->value.op);
            break;
        default:
            return;
    }
    fprintf(file, "</TD></TR>\n");

    fprintf(file, "            <TR><TD PORT=\"l\" "
                                  "COLSPAN=\"1\" "
                                  "ALIGN=\"CENTER\" "
                                  "COLOR=\"#000000\" ");
    if (node->left)
        fprintf(file, "BGCOLOR=\"%s\">%p</TD>", GenerateColor(node->left), node->left);
    else
        fprintf(file, "BGCOLOR=\"%s\">0x000000000000</TD>", GenerateColor(node->left));

    fprintf(file, "<TD PORT=\"r\" "
                  "COLSPAN=\"1\" "
                  "ALIGN=\"CENTER\" "
                  "COLOR=\"#000000\" ");
    if (node->right)
        fprintf(file, "BGCOLOR=\"%s\">%p</TD></TR>\n", GenerateColor(node->right), node->right);
    else
        fprintf(file, "BGCOLOR=\"%s\">0x000000000000</TD></TR>\n", GenerateColor(node->right));

    fprintf(file,
        "        </TABLE>\n"
        "    >];\n"
    );

    if (node->left)
    {
        ++*node_count;
        MakeNodes(node->left, node_count, file);
    }
    if (node->right)
    {
        ++*node_count;
        MakeNodes(node->right, node_count, file);
    }

    return;
}


const char* GenerateColor(const void* ptr)
{
    if (ptr == NULL) return "#cccccc";

    uintptr_t hash = (uintptr_t)ptr;
    unsigned int hue = (unsigned int) hash % 360;

    unsigned int sector = hue / 60;
    unsigned int sector_position = hue % 60;

    unsigned int r = 0, g = 0, b = 0;

    switch (sector) {
        case 0: r = 255;                              g = sector_position * 255 / 60; break; /* b = 0; */                                 // Красный → Желтый
        case 1: r = 255 - sector_position * 255 / 60; g = 255; break;                        /* b = 0; */                                 // Желтый → Зеленый
        case 2: /* r = 0; */                          g = 255;                               b = sector_position * 255 / 60; break;       // Зеленый → Голубой
        case 3: /* r = 0; */                          g = 255 - sector_position * 255 / 60;  b = 255; break;                              // Голубой → Синий
        case 4: r = sector_position * 255 / 60;       /*g = 0; */                            b = 255; break;                              // Синий → Пурпурный
        case 5: r = 255;                              /*g = 0; */                            b = 255 - sector_position * 255 / 60; break; // Пурпурный → Красный
        default: return NULL;
    }

    static char hex[8];
    snprintf(hex, sizeof(hex), "#%02x%02x%02x", r, g, b);
    return hex;
}


void MakeArrows(const Node_t* node, int* node_count, FILE* file)
{
    assert(node != NULL);
    assert(file != NULL);

    int cur_node_count = *node_count;

    if (node->left)
    {
        fprintf(file, "    node%d:<l>:s -> node%d:n [dir=normal, color=\"#4d00a6ff\"];\n",
                       cur_node_count, ++*node_count);
        MakeArrows(node->left, node_count, file);
    }
    if (node->right)
    {
        fprintf(file, "    node%d:<r>:s -> node%d:n [dir=normal, color=\"#4d00a6ff\"];\n",
                      cur_node_count, ++*node_count);
        MakeArrows(node->right, node_count, file);
    }
}


TreeErr_t WriteInHtmlFile(const Node_t* node, int count_img, const char* func, const char* file, int line)
{
    assert(node != NULL);
    assert(func != NULL);
    assert(file != NULL);

    FILE* dump_file = fopen("dump.html", "a");
    CHECK_PTR(dump_file, FILE);

    fprintf(dump_file, "<pre><b>TreeDump from %s at %s:%d\n",
                       func, file, line);

    fprintf(dump_file, "    <img src=\"TreeMemStruct/svg_dot/%ddump.svg\">\n",
                             count_img);

    fclose(dump_file);

    return TREE_OK;
}


int EndHTMLfile(void)
{
    FILE* dump_file = fopen("dump.html", "a");

    fprintf(dump_file, "</body>\n"
                       "</html>\n");

    return fclose(dump_file);
}
