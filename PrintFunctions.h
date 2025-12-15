#ifndef PRINT_FUNCTIONS
#define PRINT_FUNCTIONS

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

void BeginLaTeXDocument(FILE* file);
void DumpLaTeX(FILE* file, Node_t* node);
void PrintOriginalTree(FILE* file, Node_t* root);
void PrintCalcBegining(FILE* file);
void PrintCalcResult(FILE* file, Tree_t* tree);
void PrintDerivativeBegining(FILE* file);
void WriteTreeNodeLaTeX(FILE* file, Node_t* node);
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
void PrintPlotBinaryOperator(FILE* file, const char* const op, Node_t* node);
void PrintPlotUnaryOperator(FILE* file, const char* const op, Node_t* node);
void WriteTreeNodeGnuPlot(FILE* file, Node_t* node);

#endif//PRINT_FUNCTIONS
