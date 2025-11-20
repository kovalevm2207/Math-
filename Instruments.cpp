#include "Instruments.h"

char* ReadFile(const char* const name)
{
    assert(name != NULL && "NULL FILE NAME");

    FILE* file = fopen(name, "r");
    if(file == NULL) return NULL;

    struct stat file_info = {};
    stat(name, &file_info);
    size_t file_size = (size_t) file_info.st_size;

    char* buffer = (char*) calloc(file_size + 1, sizeof(char));
    if (buffer == NULL) {fclose(file); file = NULL; return NULL;}

    size_t ret = fread(buffer, sizeof(char), file_size, file);
    if (ret != file_size)
    {
        fclose(file);
        free(buffer);
        file = NULL;
        buffer = NULL;
        return NULL;
    }

    fclose(file);
    file = NULL;

    return buffer;
}


char* SkipSpaces(char* ptr)
{
    assert(ptr != NULL && "NUUL ptr for SkipSpaces func");

    while(*ptr && isspace((unsigned char) *ptr))
    {
        ptr++;
    }
    return ptr;
}


size_t GetWord(char** word, size_t len, char** cur_pos)
{
    assert( word);
    assert(*word);
    assert( cur_pos);
    assert(*cur_pos);

    char* buffer = *word;
    size_t start_len = len;

    size_t cur_len = 0;
    while(!isspace((unsigned char) **cur_pos) && **cur_pos != '\0')
    {
        buffer[cur_len++] = **cur_pos;
        ++*cur_pos;
        if(cur_len >= start_len)
        {
            start_len *= 2;
            char* new_buffer = (char*) realloc(buffer, start_len);
            assert(new_buffer);
            buffer = new_buffer;
        }
    }
    buffer[cur_len] = '\0';
    *word = buffer;

    return cur_len;
}
