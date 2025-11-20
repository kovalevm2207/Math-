#ifndef MATH_INSTRUMENTS
#define MATH_INSTRUMENTS

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>

#ifdef DEBUG
    #define ON_DEBUG(func) func
#else
    #define ON_DEBUG(func)
#endif

char* ReadFile(const char* const name);
char* SkipSpaces(char* ptr);
size_t GetWord(char** word, size_t len, char** cur_pos);

#endif // MATH_INSTRUMENTS
