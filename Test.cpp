#include "Types.h"

int main()
{
    Value_t left_addend = {};
    left_addend.var = 0;
    Node_t* left = TreeNodeCtor(VAR, left_addend, NULL, NULL);

    Value_t right_addend = {};
    right_addend.num = 10.5;
    Node_t* right = TreeNodeCtor(NUM, right_addend, NULL, NULL);

    Value_t plus = {};
    plus.op = SIN;
    Node_t* root = TreeNodeCtor(OP, plus, left, right);

    TreeDump(root, 1);

    DeleteTreeNode(&root);
    system("google-chroom dump.html > /dev/null 2>&1 &");

    return 0;
}
