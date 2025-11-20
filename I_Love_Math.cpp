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
        data_t word = GetAndUnuliseWord(*cur_pos);    // считываем это слово и анализируем, что нам попалось
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
            FREE(node);                               // очищаем память, если надо
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
