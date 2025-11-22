#ifndef DERIVATIVE_DSL
    #define DERIVATIVE_DSL
    #undef  UNDEF_DERIVATIVE_DSL

    #define        L node->left
    #define        R node->right
    #define        d(sub_tree) TakeDerivative(file, sub_tree, var)
    #define        c(sub_tree) DeepNodeCopy(sub_tree)
    #define        n(number)   TreeNodeCtor_(NUM, {.num = number}, NULL, NULL)
    #define     ADD_(left_sub_tree, right_sub_tree) TreeNodeCtor_(OP, {.op = ADD   }, left_sub_tree, right_sub_tree)
    #define     SUB_(left_sub_tree, right_sub_tree) TreeNodeCtor_(OP, {.op = SUB   }, left_sub_tree, right_sub_tree)
    #define     MUL_(left_sub_tree, right_sub_tree) TreeNodeCtor_(OP, {.op = MUL   }, left_sub_tree, right_sub_tree)
    #define     DIV_(left_sub_tree, right_sub_tree) TreeNodeCtor_(OP, {.op = DIV   }, left_sub_tree, right_sub_tree)
    #define     POW_(left_sub_tree, right_sub_tree) TreeNodeCtor_(OP, {.op = POW   }, left_sub_tree, right_sub_tree)
    #define     LOG_(left_sub_tree, right_sub_tree) TreeNodeCtor_(OP, {.op = LOG   }, left_sub_tree, right_sub_tree)
    #define    SQRT_(left_sub_tree                ) TreeNodeCtor_(OP, {.op = SQRT  }, left_sub_tree, NULL          )
    #define     SIN_(left_sub_tree                ) TreeNodeCtor_(OP, {.op = SIN   }, left_sub_tree, NULL          )
    #define     COS_(left_sub_tree                ) TreeNodeCtor_(OP, {.op = COS   }, left_sub_tree, NULL          )
    #define      TG_(left_sub_tree                ) TreeNodeCtor_(OP, {.op = TG    }, left_sub_tree, NULL          )
    #define     CTG_(left_sub_tree                ) TreeNodeCtor_(OP, {.op = CTG   }, left_sub_tree, NULL          )
    #define      SH_(left_sub_tree                ) TreeNodeCtor_(OP, {.op = SH    }, left_sub_tree, NULL          )
    #define      CH_(left_sub_tree                ) TreeNodeCtor_(OP, {.op = CH    }, left_sub_tree, NULL          )
    #define      TH_(left_sub_tree                ) TreeNodeCtor_(OP, {.op = TH    }, left_sub_tree, NULL          )
    #define     CTH_(left_sub_tree                ) TreeNodeCtor_(OP, {.op = CTH   }, left_sub_tree, NULL          )
    #define  ARCSIN_(left_sub_tree                ) TreeNodeCtor_(OP, {.op = ARCSIN}, left_sub_tree, NULL          )
    #define  ARCCOS_(left_sub_tree                ) TreeNodeCtor_(OP, {.op = ARCCOS}, left_sub_tree, NULL          )
    #define   ARCTG_(left_sub_tree                ) TreeNodeCtor_(OP, {.op = ARCTG }, left_sub_tree, NULL          )
    #define  ARCCTG_(left_sub_tree                ) TreeNodeCtor_(OP, {.op = ARCCTG}, left_sub_tree, NULL          )
    #define      LG_(left_sub_tree                ) TreeNodeCtor_(OP, {.op = LG    }, left_sub_tree, NULL          )
    #define      LN_(left_sub_tree                ) TreeNodeCtor_(OP, {.op = LN    }, left_sub_tree, NULL          )
    #define COMP_FUNC(func) MUL_(func, d(L))


#endif // DERIVATIVE_DSL
