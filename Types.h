#ifndef MATH_TYPES
#define MATH_TYPES

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <string.h>

#include "TreeMemStruct/Tree.h"
#include "color_print.h"
#include "Instruments.h"

const char* const phrases[] =
{
    "Не трудно заметить, что:",
    "И любому ёжику понятно что здесь получится:",
    "Это достаточно просто преобразуется в следующее выражение:",
    "Как вы помните еще с рождения:",
    "Давайте не будем слишком долго останавливаться на таких простых преобразованиях и просто сразу напишем:",
    "Вопросы есть откуда это взялось?:",
    "Собственно из этого уже мы легко получаем, что:",
    "Ну тут вообще обсуждать нечего:",
    "Ээээээээээ... ну тут короче так:",
    "Не спрашивайте вообще, как это у нас получилось:",
    "После долгих мучительных преобразований мы наконец получаем:",
    "Я надеюсь, что вы все поняли, но в общем вот:",
    "Так, ну тут вообще ничего интересного:",
    "Па па па па па папапапапа па:",
    "Теперь по известным всем вам правилам переходим к следующему выражению",
    "Тыры-пыры, шуры-муры и вот оно:"
};

const long unsigned int PHRASES_NUM = sizeof(phrases)/sizeof(phrases[0]);

#define FREE(ptr) if(ptr) {free(ptr); ptr = NULL;}

//SECTION -  reading
Node_t* GetTreeNode(char** cur_pos);
data_t* GetAndUnulyzeWord(char** cur_pos, data_t* data);
data_t* AnalyzeWord(char* word, data_t* data);
bool FindOperator(char* word, Operator_t* op);

//SECTION -  write LaTeX
void BeginLaTeXDocument(FILE* file);
void DumpLaTeX(FILE* file, Node_t* node);
void WriteTreeNodeLaTeX(FILE* file, Node_t* node);
bool NeedBraces(Node_t* node, Node_t* next_node);
void PrintLog_(FILE* file, Node_t* node);
void PrintDiv_(FILE* file, Node_t* node);
void PrintPow_(FILE* file, Node_t* node);
void PrintSqrt_(FILE* file, Node_t* node);
void PrintBinaryOperator_(FILE* file, const char* const op, Node_t* node);
void PrintUnaryOperator_(FILE* file, const char* const op, Node_t* node);
void EndLaTeXDocument(FILE* file);
#define PrintLog()  PrintLog_(file, node)
#define PrintDiv()  PrintDiv_(file, node)
#define PrintPow()  PrintPow_(file, node)
#define PrintSqrt() PrintSqrt_(file, node)
#define PrintBinaryOperator(op) PrintBinaryOperator_(file, op, node)
#define PrintUnaryOperator(op)  PrintUnaryOperator_(file, op, node)

//SECTION -  derivative
Node_t* TakeDerivative(FILE* file, Node_t* node, const char* const var);
Node_t* DeepNodeCopy(Node_t* node);
bool FindVar(Node_t* node, const char* const var);
Node_t* PowDerivative(FILE* file, Node_t* node, const char* const var);
Node_t* LogDerivative(FILE* file, Node_t* node, const char* const var);

#endif // MATH_TYPES
