#ifndef READING_USER_FILE
#define READING_USER_FILE

#include "Instruments.h"

Node_t* GetTreeNode(char** cur_pos);
data_t* GetAndUnulyzeWord(char** cur_pos, data_t* data);
data_t* AnalyzeWord(char* word, data_t* data);

// have q? ---> read Rules.txt
Node_t* GetG(char** s);
Node_t* GetE(char** s);
Node_t* GetT(char** s);
Node_t* GetS(char** s);
Node_t* GetP(char** s);
Node_t* GetA(char** s);
Node_t* GetN(char** s);
char* GetW(char** s);

#endif//READING_USER_FILE
