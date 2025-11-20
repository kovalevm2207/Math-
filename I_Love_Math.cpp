#include "Types.h"

int main()
{
    // reading from file
    char* user_file = ReadFile("Expreshion.txt");
    assert(user_file && "NULL user_file, check ReadFile func");
    char* cur_pos = SkipSpaces(user_file);

    Node_t* user_tree = GetTreeNode(&cur_pos);
    ON_DEBUG(printf("user_tree = %p\n", user_tree));
    TreeDump(user_tree, 1);

    DeleteTreeNode(&user_tree);
    FREE(user_file)

    return 0;
}


Node_t* GetTreeNode(char** cur_pos)
{
    ON_DEBUG(printf("In func GetTreeNode\n"));
    ON_DEBUG(printf("\tat the begining we are on sym: %c (%d)\n", **cur_pos, **cur_pos));
    assert( cur_pos && "NULL ptr on buffer ptr, check GetTreeNode func");
    assert(*cur_pos && "NULL ptr on buffer, check SkipSpaces func");

    if(**cur_pos == '(')
    {
        ON_DEBUG(printf("\thave found (\n"));
        *cur_pos = SkipSpaces(++*cur_pos);
        ON_DEBUG(printf("\tnow we are on %c\n", **cur_pos));

        data_t word = {};
        GetAndUnulyzeWord(cur_pos, &word);
        ON_DEBUG(printf("\tafter retunr from analyze we have type: %s(%d)\n", NodeTypes[word.type], word.type));

        ON_DEBUG(printf("\tbefore TreeNodeCtore we are on: %c (%d)\n", **cur_pos, **cur_pos));
        Node_t* left_som  = GetTreeNode(cur_pos);
        Node_t* right_som = GetTreeNode(cur_pos);
        Node_t* node = TreeNodeCtor(&word, left_som, right_som);
        if (!node) return NULL;
        ON_DEBUG(printf("\tafter TreeNodeCtor we have: ptr = %p\n", node));
        ON_DEBUG(printf("\tafter TreeNodeCtor we are on symbol: %c (%d)\n", **cur_pos, **cur_pos));

        if (**cur_pos == ')')
        {
            *cur_pos = SkipSpaces(++*cur_pos);
            return node;
        }
        else
        {
            FREE(node)
            *cur_pos = SkipSpaces(++*cur_pos);
            return NULL;
        }
    }
    else if(strncmp(*cur_pos, "nil", 3) == 0)
    {
        ON_DEBUG(printf("\thave found: nil\n"));

        *cur_pos += sizeof("nil") - 1;
        *cur_pos = SkipSpaces(*cur_pos);

        ON_DEBUG(printf("After skip 'nil' and spaces we are on: %c (%d)\n", **cur_pos, **cur_pos));
        ON_DEBUG(printf("Out from GetTreeNode"));

        return NULL;
    }

    ON_DEBUG(printf(RED_COLOR "\tinvalid word in txt file\n" RESET));

    return NULL;
}


data_t* GetAndUnulyzeWord(char** cur_pos, data_t* data)
{
    assert( cur_pos);
    assert(*cur_pos);
    const size_t START_LEN = 1 << 4;

    char* word = (char*) calloc(START_LEN, sizeof(char));
    assert(word && "memmory allocation err");

    if (!GetWord(&word, START_LEN, cur_pos))
    {
        FREE(word)
        return NULL;
    }
    ON_DEBUG(printf("\twe have read: %s\n", word));

    UnulyzeWord(word, data);
    ON_DEBUG(printf("\tafter analyze we have type: %s(%d)\n", NodeTypes[data->type], data->type));
    FREE(word);
    *cur_pos = SkipSpaces(*cur_pos);
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

    Operator_t check_op = ADD;
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
            *op = (Operator_t) i;
            return true;
        }
    }

    return false;
}
