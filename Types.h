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

const char* const TakeDerivativePhrases[] =
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
    "Теперь по известным всем вам правилам переходим к следующему выражению:",
    "Тыры-пыры, шуры-муры и вот оно:",
    "Дифференцируем как боги:",
    "Производная? Легко! Смотрите:",
    "Берем производную на скорость:",
    "Так, тут производная будет... эээ... ну вот:",
    "По правилам дифференцирования, которые вы все знаете наизусть:",
    "Дифференцируем не глядя:",
    "Производная как с конвейера:",
    "Дифференцируем по-быстрому:",
    "Ну это же элементарно, Ватсон:",
    "Секундочку... вычисляем... готово:",
    "Дифференцируем на автопилоте:",
    "Производная готова, можете забрать:",
    "Так, щас быстренько продифференцировать... вжух:"
};

const char* const ConstantFoldingPhrases[] =
{
    "Ну наконец-то хоть что-то посчиталось:",
    "Опа, а тут числа сами посчитались!:",
    "Вот это да! Калькулятор сработал!:",
    "Surprise можно было и не писать столько:",
    "Долго ли, коротко ли, но посчиталось:",
    "Вы не поверите, но это упрощается именно так:",
    "Секретная техника вычислений раскрыта:",
    "Вот что бывает, когда числа подчиняются законам математики",
    "Смотрите-ка, а тут можно посчитать!",
    "Зачем нам лишние вычисления? Вот проще:",
    "Да это ж в уме посчитать можно:",
    "Калькулятор в руки взял и получил:",
    "Тут даже ИИ не нужен, обычный калькулятор справится:",
    "Упрощаем по-хардкору:",
    "Считаем в уме... раз... два... готово:",
    "Это как 2+2, только сложнее:",
    "Вот что получается, когда числа дружат:",
    "Магия! Исчезли лишние действия:",
    "Так-так-так, что тут у нас... ага:",
    "Сокращаем-сокращаем... и вот:",
    "Вычисляем без регистрации и СМС, мамка не запалит:",
    "Ну тут всё очевидно, даже комментировать не буду:",
    "Вжух-вжух и готово:",
    "Как по маслу пошло:"
};


const long unsigned int TAKE_DERIVATIVE_PHRASES_NUM = sizeof(TakeDerivativePhrases)/sizeof(TakeDerivativePhrases[0]);
const long unsigned int CONST_FOLDING_PHRASES_NUM   = sizeof(ConstantFoldingPhrases)/sizeof(ConstantFoldingPhrases[0]);
const double PRECISION = 1e-12;
const size_t ST_W_LEN = 5;

//SECTION -  reading
Node_t* GetTreeNode(char** cur_pos);
data_t* GetAndUnulyzeWord(char** cur_pos, data_t* data);
data_t* AnalyzeWord(char* word, data_t* data);
bool FindOperator(char* word, Operator_t* op);
TreeErr_t MakePrevNode(Node_t* node);

//SECTION -  write LaTeX
void BeginLaTeXDocument(FILE* file);
void DumpLaTeX(FILE* file, Node_t* node);
void PrintOriginalTree(FILE* file, Node_t* root);
void PrintCalcBegining(FILE* file);
void PrintCalcResult(FILE* file, Tree_t* tree);
void PrintDerivativeBegining(FILE* file);
void WriteTreeNodeLaTeX(FILE* file, Node_t* node);
bool NeedBraces(Node_t* node, Node_t* next_node);
void PrintLog_(FILE* file, Node_t* node);
void PrintDiv_(FILE* file, Node_t* node);
void PrintPow_(FILE* file, Node_t* node);
void PrintSqrt_(FILE* file, Node_t* node);
void PrintBinaryOperator_(FILE* file, const char* const op, Node_t* node);
void PrintUnaryOperator_(FILE* file, const char* const op, Node_t* node);
void PrintBeginSimplify(FILE* file);
void PrintSimplifyRes(FILE* file, Node_t* base_node, Node_t* simple_node, const char* const var, bool flag);
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

//SECTION - calc
double CalcTree(Tree_t* tree);
TreeErr_t GetVarsValues(Tree_t* tree);
double CalcTreeNode(const Node_t* node, const Var_t* vars, size_t vars_num  );
size_t FindVarPos(const char* const name, const Var_t* const vars, size_t vars_num);

//SECTION - simplify expression
TreeErr_t SimplifyExpression(FILE* file, Tree_t* base_tree, Tree_t* tree, int* count_img);
Node_t* ConstantFolding(Node_t** node_, bool* is_change);
Node_t* UnaryConstantFolding(Node_t** node_, bool* is_change);
Node_t* BinaryConstantFolding(Node_t** node_, bool* is_change);
Node_t* NeutralElementElimination(Node_t** node, bool* is_change);
Node_t* SimplifyAdd(Node_t** node_, bool* is_change);
Node_t* SimplifySub(Node_t** node_, bool* is_change);
Node_t* SimplifyMul(Node_t** node_, bool* is_change);
Node_t* SimplifyPow(Node_t** node_, bool* is_change)
int DoubleCompare(double a, double b);

Node_t* GetG(char** s);
Node_t* GetE(char** s);
Node_t* GetT(char** s);
Node_t* GetS(char** s);
Node_t* GetP(char** s);
Node_t* GetA(char** s);
Node_t* GetN(char** s);
char* GetW(char** s);
Operator_t FindOp(char* const word);

double SkipSpaces(char** s);
size_t CheckLen(char** word, size_t word_len, const size_t letter);

#define FREE(ptr) if(ptr) {free(ptr); ptr = NULL;}
#define ERR_PRINT(text) fprintf(stderr, RED_COLOR text RESET)

#endif // MATH_TYPES
