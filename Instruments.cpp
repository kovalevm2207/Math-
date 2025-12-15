#include "Instruments.h"

char* ReadFile(const char* const name)
{
    assert(name != NULL && "NULL FILE NAME");

    FILE* file = fopen(name, "r");
    if(file == NULL) return NULL;

    struct stat file_info = {};
    stat(name, &file_info);
    size_t file_size = (size_t) file_info.st_size;

    char* buffer = (char*) calloc(file_size + 1, sizeof(char));
    if (buffer == NULL) {fclose(file); file = NULL; return NULL;}

    size_t ret = fread(buffer, sizeof(char), file_size, file);
    if (ret != file_size)
    {
        fclose(file);
        free(buffer);
        file = NULL;
        buffer = NULL;
        return NULL;
    }

    fclose(file);
    file = NULL;

    return buffer;
}
char* SkipSpaces(char* ptr)
{
    assert(ptr != NULL && "NUUL ptr for SkipSpaces func");

    while(*ptr && isspace((unsigned char) *ptr))
    {
        ptr++;
    }
    return ptr;
}
size_t GetWord(char** word, size_t len, char** cur_pos)
{
    assert( word);
    assert(*word);
    assert( cur_pos);
    assert(*cur_pos);

    char* buffer = *word;
    size_t start_len = len;

    size_t cur_len = 0;
    while(!isspace((unsigned char) **cur_pos) && **cur_pos != '\0')
    {
        buffer[cur_len++] = **cur_pos;
        ++*cur_pos;
        if(cur_len >= start_len)
        {
            start_len *= 2;
            char* new_buffer = (char*) realloc(buffer, start_len);
            assert(new_buffer);
            buffer = new_buffer;
        }
    }
    buffer[cur_len] = '\0';
    *word = buffer;

    return cur_len;
}
size_t CheckLen(char** word, size_t word_len, const size_t letter)
{
    assert(word);
    assert(*word);
    assert(letter <= word_len);

    if(letter == word_len)
    {
        char* new_word = (char*) realloc(*word, (word_len * 2) * sizeof(char));
        assert(new_word);
        *word = new_word;
        memset(*word + word_len, 0, word_len);
        word_len *= 2;
    }

    return word_len;
}
Operator_t FindOp(char* const word)
{
    assert(word);

    for(size_t i = 0; i < operators_num; i++)
    {
        if(strcmp(word, Operators[i].symbol) == 0)
            return (Operator_t) i;
    }

    return NOT_OP;
}
int DoubleCompare(double a, double b)
{
    if(fabs(a - b) < PRECISION) return 0;
    return a < b ? -1 : 1;
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
size_t FindVarPos(const char* const name, const Var_t* const vars, size_t vars_num)
{
    assert(name);
    assert(vars);

    for(size_t pos = 0; pos < vars_num; pos++)
    {
        if(strcmp(name, vars[pos].name) == 0) return pos;
    }

    assert(false);
    return 0;
}
TreeErr_t GetVarsValues(Tree_t* tree)
{
    assert(tree);

    for(size_t i = 0; i < tree->vars_num; i++)
    {
        printf("Введите значение переменной %s:\n\n\t\t", tree->vars[i].name);
        scanf("%lg", &tree->vars[i].data);
        printf("\n\n");
    }

    return TREE_OK;
}
bool FindVar(Node_t* node, const char* const var)
{
    assert(node);
    assert(var);

    switch(node->node_type)
    {
        case VAR:
            if (strcmp(node->value.var, var) == 0) return true;
            return false;
        case NUM:
            return false;
        case OP:
            if(node->left)
            {
                if (FindVar(node->left, var)) return true;
            }
            else
            {
                assert(false);
                return false;
            }

            if(node->right)
            {
                if (FindVar(node->right, var)) return true;
            }
            return false;
        default:
            assert(false);
            return false;
    }
}
bool NeedBraces(Node_t* node, Node_t* next_node)
{
    assert(node);
    assert(next_node);

    ON_DEBUG(printf("node:      type = %s op = %s\n", NodeTypes[node->node_type], Operators[node->value.op].name));
    ON_DEBUG(printf("next_node: type = %s\n", NodeTypes[next_node->node_type]));
    switch(next_node->node_type)
    {
        case NUM:
        case VAR:
            return false;
        case OP:
            if(node->value.op == POW && node->left->node_type == OP && node->left->value.op == POW)
                   return true;
            if(Operators[next_node->value.op].priority < Operators[node->value.op].priority)
                return true;
            return false;
        default:
            return false;
    }
    return false;
}
double CalcTree(Tree_t* tree)
{
    assert(tree);

    GetVarsValues(tree);
    TreeStructDump(tree);

    return CalcTreeNode(tree->root, tree->vars, tree->vars_num);
}
double CalcTreeNode(const Node_t* node, const Var_t* vars, size_t vars_num)
{
    assert(node);
    assert(vars);

    switch(node->node_type)
    {
        default:
            assert(false);
            return 0;
        case NUM:
            return node->value.num;
        case VAR:
            return vars[FindVarPos(node->value.var, vars, vars_num)].data;
        case OP:
            double result = 0;

            #define LC CalcTreeNode(node->left,  vars, vars_num)
            #define RC CalcTreeNode(node->right, vars, vars_num)

            switch(node->value.op)
            {
                case NOT_OP:
                default:
                    assert(false);
                    return 0;
                case ADD:       result = LC + RC;           break;
                case SUB:       result = LC - RC;           break;
                case MUL:       result = LC * RC;           break;
                case DIV:       result = LC / RC;           break;
                case SQRT:      result = sqrt(LC);          break;
                case SIN:       result = sin(LC);           break;
                case COS:       result = cos(LC);           break;
                case TG:        result = tan(LC);           break;
                case CTG:       result = 1/tan(LC);         break;
                case SH:        result = sinh(LC);          break;
                case CH:        result = cosh(LC);          break;
                case TH:        result = tanh(LC);          break;
                case CTH:       result = 1/tanh(LC);        break;
                case ARCSIN:    result = asin(LC);          break;
                case ARCCOS:    result = acos(LC);          break;
                case ARCTG:     result = atan(LC);          break;
                case ARCCTG:    result = M_PI_2 - atan(LC); break;
                case LG:        result = log10(LC);         break;
                case LN:        result = log(LC);           break;
                case LOG:       result = log(LC)/log(RC);   break;
                case POW:       result = pow(LC, RC);       break;
            }

            #undef LC
            #undef RC

            return result;

            break;
    }

    assert(false);
    return 0;
}
