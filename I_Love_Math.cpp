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


Node_t* GetTreeNode(char** cur_pos)
{
    assert( cur_pos && "NULL ptr on buffer ptr, check GetTreeNode func");
    assert(*cur_pos && "NULL ptr on buffer, check SkipSpaces func");

    return NULL;
}
