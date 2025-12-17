#ifndef READING_USER_FILE
#define READING_USER_FILE

#include "Instruments.h"

const double BASE_TAYLOR_ORDER = 7;
const double BASE_EXPANSION_DOT = 0;
const double BASE_LEFT_X_SCALE = 10;
const double BASE_RIGHT_X_SCALE = 10;
const double BASE_LEFT_Y_SCALE = -5;
const double BASE_RIGHT_Y_SCALE = 5;

ProgramData_t GetInputData(const char* const file_name);
Tree_t* GetExpression(const char* const file_name);
Node_t* GetTreeNode(char** cur_pos);
data_t* GetAndUnulyzeWord(char** cur_pos, data_t* data);
data_t* AnalyzeWord(char* word, data_t* data);

// have q? ---> read Rules.txt
ProgramData_t GetG(char** s);
Scale_t GetY_S(char** s);
Scale_t GetX_S(char** s);
double GetT_O(char** s);
double GetE_D(char** s);
Node_t* GetF(char** s);
Node_t* GetE(char** s);
Node_t* GetT(char** s);
Node_t* GetS(char** s);
Node_t* GetP(char** s);
Node_t* GetA(char** s);
double GetN(char** s);
char* GetW(char** s);

#endif//READING_USER_FILE
