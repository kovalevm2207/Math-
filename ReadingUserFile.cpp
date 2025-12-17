#include "ReadingUserFile.h"


Tree_t* GetExpression(const char* const file_name)
{
    assert(file_name);

    char* user_file = ReadFile(file_name);
    assert(user_file && "NULL user_file, check ReadFile func");

    char* cur_pos = SkipSpaces(user_file);

    Node_t* user_nodes = GetG(&cur_pos);
    MakePrevNode(user_nodes);
    FREE(user_file)

    return TreeCtor(user_nodes);
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

    AnalyzeWord(word, data);
    ON_DEBUG(printf("\tafter analyze we have type: %s(%d)\n", NodeTypes[data->type], data->type));
    FREE(word);
    *cur_pos = SkipSpaces(*cur_pos);
    return data;
}
data_t* AnalyzeWord(char* word, data_t* data)
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

Node_t* GetG(char** s)
{
    assert(s);
    assert(*s);

    *s = SkipSpaces(*s);
    Node_t* root = GetE(s);
    *s = SkipSpaces(*s);

    if((**s) != '\0')
    {
        //DeleteTreeNode(&root);
        ERR_PRINT("SyntaxErr in GetG\n");
        return NULL;
    }
    return root;
}
Node_t* GetE(char** s)
{
    assert(s);
    assert(*s);

    *s = SkipSpaces(*s);
    Node_t* node = GetT(s);
    while((**s) == '+' || (**s) == '-')
    {
        int op = (**s);
        (*s)++;
        Node_t* r_node = GetT(s);
        if(!r_node)
        {
        //    DeleteTreeNode(&node);
            return NULL;
        }
        node = TreeNodeCtor_(OP, {.op = (op == '+') ? ADD : SUB}, node, r_node);
    }
    return node;
}
Node_t* GetT(char** s)
{
    assert(s);
    assert(*s);

    *s = SkipSpaces(*s);
    Node_t* node = GetS(s);

    *s = SkipSpaces(*s);
    while((**s) == '*' || (**s) == '/')
    {
        int op = (**s);
        (*s)++;
        Node_t* r_node = GetS(s);
        if(!r_node)
        {
        //    DeleteTreeNode(&node);
            return NULL;
        }
        node = TreeNodeCtor_(OP, {.op = (op == '*') ? MUL : DIV}, node, r_node);
    }
    return node;
}
Node_t* GetS(char** s)
{
    assert(s);
    assert(*s);

    *s = SkipSpaces(*s);
    Node_t* l_node = GetP(s);
    *s = SkipSpaces(*s);

    if(**s == '^')
    {
        (*s)++;
        *s = SkipSpaces(*s);
        Node_t* r_node = GetS(s);
        if(r_node == NULL)
        {
        //    DeleteTreeNode(&l_node);
            return NULL;
        }
        return TreeNodeCtor_(OP, {.op = POW}, l_node, r_node);
    }
    return l_node;
}
Node_t* GetP(char** s)
{
    assert(s);
    assert(*s);

    *s = SkipSpaces(*s);

    if((**s) == '(')
    {
        (*s)++;
        *s = SkipSpaces(*s);
        Node_t* node = GetE(s);

        *s = SkipSpaces(*s);
        if((**s) == ')')
        {
            (*s)++;
            *s = SkipSpaces(*s);
            return node;
        }
        else
        {
            ERR_PRINT("SyntaxErr in GetP after '('\n");
            DeleteTreeNode(&node);
        }
    }
    else if(**s == '-' || ('0' <= **s && **s <= '9')) return GetN(s);
    else
    {
        char* word = GetW(s);
        Operator_t op = FindOp(word);
        if(op == NOT_OP) return TreeNodeCtor_(VAR, {.var = word}, NULL, NULL);
        else
        {
            FREE(word);
            if(**s == '(')
            {
                (*s)++;
                Node_t* args = GetA(s);
                assert(args && "SyntaxErr");
                *s = SkipSpaces(*s);
                if(**s == ')')
                {
                    (*s)++;
                    Node_t* node = TreeNodeCtor_(OP, {.op = op}, args->left, args->right);
                    args->left->prev_node = NULL;
                    if(args->right) args->right->prev_node = NULL;
                    args->left = args->right = NULL;
                    DeleteTreeNode(&args);
                    return node;
                }
                ERR_PRINT("SyntaxErr in GetP apsent ')'\n");
                DeleteTreeNode(&args);
            }
        }
    }

    return NULL;
}
Node_t* GetA(char** s)
{
    assert(s);
    assert(*s);

    *s = SkipSpaces(*s);

    Node_t* first_arg = GetE(s);
    if(!first_arg)
    {
        ERR_PRINT("SyntaxErr\n");
        return NULL;
    }
    *s = SkipSpaces(*s);

    Node_t* sec_arg = NULL;
    if(**s == ',')
    {
        (*s)++;
        *s = SkipSpaces(*s);
        sec_arg = GetE(s);
        if(!sec_arg)
        {
            ERR_PRINT("SyntaxErr\n");
            return NULL;
        }
    }

    return TreeNodeCtor_(OP, {.op = NOT_OP}, first_arg, sec_arg);
}
Node_t* GetN(char** s)
{
    assert(s);
    assert(*s);

    *s = SkipSpaces(*s);
    double sign = 1;
    if(**s == '-')
    {
        sign = -1;
        (*s)++;
    }
    if(!('0'<=(**s) && (**s)<='9'))
    {
        ERR_PRINT("SyntaxErr\n");
        return NULL;
    }

    double val = 0;
    while('0'<=(**s) && (**s)<='9')
    {
        val = val*10 + ((**s) - '0');
        (*s)++;
    }

    if(**s == '.')
    {
        (*s)++;

        double frac_part = 0;
        double order = 1;
        while('0'<=(**s) && (**s)<='9')
        {
            frac_part += ((**s) - '0') / pow(10, order++);
            (*s)++;
        }

        val += frac_part;
    }
    return TreeNodeCtor_(NUM, {.num = val * sign}, NULL, NULL);
}
char* GetW(char** s)
{
    assert(s);
    assert(*s);

    char* word = (char*) calloc(ST_W_LEN, sizeof(char));
    assert(word && "mem alloc err (calloc)");

    size_t letter = 0;
    size_t word_len = ST_W_LEN;

    *s = SkipSpaces(*s);
    while(('A' <= (**s) && (**s) <= 'Z') ||
          ('a' <= (**s) && (**s) <= 'z'))
    {
        word[letter++] = **s;
        (*s)++;

        word_len = CheckLen(&word, word_len, letter);
        assert(word_len);
    }
    if(letter == 0)
    {
        free(word);
        return NULL;
    }

    while(('A' <= (**s) && (**s) <= 'Z') ||
          ('a' <= (**s) && (**s) <= 'z') ||
          ('0' <= (**s) && (**s) <= '9') ||
          (**s) == '_')
    {
        word[letter++] = **s;
        (*s)++;

        word_len = CheckLen(&word, word_len, letter);
        assert(word_len);
    }

    word[letter] = '\0';

    return word;
}
