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
        switch(*ptr)
        {
            case '\n':
                ON_DEBUG(printf("enter\n"));
                break;
            case ' ':
                ON_DEBUG(printf("space\n"));
                break;
            default:
                ON_DEBUG(printf("%c", *ptr));
                break;
        }

        ptr++;
    }
    return ptr;
}
