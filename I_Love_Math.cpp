#include "Types.h"

int main()
{
    // reading from file
    char* user_file = ReadFile("Expreshion.txt");
    assert(user_file && "NULL user_file, check ReadFile func");
    char* cur_pos = SkipSpaces(user_file);

    Node_t* user_tree = GetTreeNode(&cur_pos);

    FREE(user_file)

    return 0;
}

// после себя всегда оставляет указатель на не пробельный символ
Node_t* GetTreeNode(char** cur_pos)
{
    assert( cur_pos && "NULL ptr on buffer ptr, check GetTreeNode func");
    assert(*cur_pos && "NULL ptr on buffer, check SkipSpaces func");

    if(**cur_pos == '(')                              // мы могли встретить новый узел
    {                                                 // тогда:
        *cur_pos = SkipSpaces(++*cur_pos);            // пропустили скобку, потом пробелы после скобки
                                                      // теперь мы на первом символе слова
        data_t word = {};
        GetAndUnulyzeWord(*cur_pos, &word);           // считываем это слово и анализируем, что нам попалось
        Node_t* node = TreeNodeCtor(word, GetTreeNode(cur_pos), GetTreeNode(cur_pos)); // создаем узел с нужным значнием и прикрепляем к нему поддеревья
        if (!node) return NULL;                       // не забываем сделать проверку, на то, что узел успешно создался
        // т.к. GetTreeNode после себя оставляет указатель на непробельный символ, то мы должны встретить ')' после прочтения поддеревьев
        if (*cur_pos == ')')
        {  // все хорошо, узел считался правильно
            *cur_pos = SkipSpaces(++*cur_pos);        // пропускаем скобку и пробельные символы послее нее
            return node;                              // возвращаем указатель на получившийся узел
        }
        else
        {  // все плохо, узел считался неправильно
            FREE(node)                                // очищаем память, если надо
            *cur_pos = SkipSpaces(++*cur_pos);        // пропускаем скобку и пробельные символы послее нее
            return NULL;
        }                                             // чтение узла закончили
    }
    else if(strncmp(*cur_pos, "nil") == 0)            // но также мы могли встретить еще и "nil"
    {                                                 // тогда:
        *cur_pos += sizeof("nil") - 1;                // прпускаем "nil"
        *cur_pos = SkipSpaces(*cur_pos);              // пропускаем все пробельные символы
        return NULL;
    }
    else return NULL;                                 // во всех остальных случаях возвращаем NULL, не смещаясь с некорректного символа
}


// после себя всегда оставляет указатель на не пробельный символ
data_t* GetAndUnulyzeWord(char** cur_pos, data_t* data)
{
    assert( cur_pos);
    assert(*cur_pos);
    const size_t START_LEN = 1 << 4;

    char* word = (char*) calloc(START_LEN, sizeof(char));
    assert(word && "memmory allocation err");

    if (!GetWord(&word, START_LEN, cur_pos)) // после себя всегда оставляет указатель на не пробельный символ
    {
        FREE(word)
        return NULL;
    }

    UnulyzeWord(word, data);
    FREE(word);
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

    Operator_t check_op = 0;
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
            *op = i;
            return true;
        }
    }

    return false;
}
