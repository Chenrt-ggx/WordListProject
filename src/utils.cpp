#include "utils.h"
#include "errors.h"

#include <io.h>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <cstdlib>

bool is_txt(const char* s)
{
    int len = (int)strlen(s);
    return len >= 4 && strstr(s + len - 4, ".txt") != nullptr;
}

void set_flag(bool& flag)
{
    if (flag)
    {
        throw E_FLAG_CONFLICT;
    }
    flag = true;
}

void parse_alpha(const char* s, char& param)
{
    if (strlen(s) != 1)
    {
        throw E_FLAG_PARAM_FORMAT_ERROR;
    }
    if (!isalpha(*s))
    {
        throw E_FLAG_PARAM_FORMAT_ERROR;
    }
    param = tolower(*s);
}

void read_file(const char* filename, char*& content, int& len)
{
    FILE* file;
    if (_access(filename, 0))
    {
        throw E_FILE_NOT_EXIST;
    }
    if (fopen_s(&file, filename, "rb") || file == nullptr)
    {
        throw E_FILE_UNABLE_TO_OPEN;
    }
    fseek(file, 0, SEEK_END);
    len = (int)ftell(file);
    fseek(file, 0, SEEK_SET);
    content = (char*)malloc((size_t)len + 1);
    if (content == nullptr)
    {
        throw E_MEMORY_ALLOC_FAILED;
    }
    size_t read = fread(content, 1, (size_t)len + 1, file);
    if (read != len)
    {
        throw E_FILE_MODIFIED_WHEN_READ;
    }
    content[read] = 0;
    fclose(file);
}

int split_content(char* content, const int len, char* result[])
{
    for (char* c = content; *c != '\0'; c++)
    {
        if ('A' <= *c && *c <= 'Z')
        {
            *c = *c + 32;
        }
        else if (*c < 'a' || *c > 'z')
        {
            *c = '\0';
        }
    }
    int count = 0;
    for (int i = 0; i < len; i++)
    {
        if (content[i] != '\0' && content[i + 1] != '\0')
        {
            result[count++] = content + i;
            while (content[i] != '\0')
            {
                i++;
            }
        }
    }
    return count;
}

void write_to_screen(char* result[], const int len)
{
    printf("%d\n", len);
    if (len <= 20000)
    {
        for (int i = 0; i < len; i++)
        {
            puts(result[i]);
        }
    }
    else
    {
        puts("output terminated: buffer length 20000 exceed");
    }
}

void write_to_solution(char* result[], const int len)
{
    FILE* file;
    if (fopen_s(&file, "solution.txt", "w") || file == nullptr)
    {
        throw E_FILE_UNABLE_TO_OPEN;
    }
    if (len <= 20000)
    {
        for (int i = 0; i < len; i++)
        {
            fputs(result[i], file);
            fputc('\n', file);
        }
    }
    else
    {
        fputs("output terminated: buffer length 20000 exceed", file);
        fputc('\n', file);
    }
    fclose(file);
}
