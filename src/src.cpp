﻿#include <cstdio>
#include <cctype>
#include <cstring>
#include <cstdlib>
#include <vector>

#include "core.h"
#include "global.h"

#include "../core/core.h"
#pragma comment(lib, "../x64/Debug/core.lib")

_declspec(dllimport) int add(int a, int b);
_declspec(dllimport) int mul(int a, int b);

using namespace std;

bool flag_n = false;
bool flag_w = false;
bool flag_m = false;
bool flag_c = false;
bool flag_h = false;
bool flag_t = false;
bool flag_r = false;

char param_h = '\0';
char param_t = '\0';

char* filename = nullptr;

inline bool is_txt(char* s)
{
    size_t len = strlen(s);
    return len >= 4 && strstr(s + len - 4, ".txt") != nullptr;
}

inline void set_flag(bool& flag)
{
    if (flag)
    {
        throw E_FLAG_CONFLICT;
    }
    flag = true;
}

inline void parse_alpha(char* s, char& param)
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

inline void conflict_check()
{
    int sum = flag_n + flag_w + flag_m + flag_c;
    if (sum == 0)
    {
        throw E_FLAG_NOT_EXIST;
    }
    if (sum > 1)
    {
        throw E_FLAG_CONFLICT;
    }
    if (flag_n && (flag_h || flag_t || flag_r))
    {
        throw E_FLAG_CONFLICT;
    }
    if (flag_m && (flag_h || flag_t || flag_r))
    {
        throw E_FLAG_CONFLICT;
    }
    if (filename == nullptr) {
        throw E_FILENAME_NOT_PROVIDE;
    }
}

inline void parse_params(int argc, char* argv[]) 
{
    for (int i = 1; i < argc; i++)
    {
        char* arg = argv[i];
        if (is_txt(arg)) 
        {
            if (filename != nullptr)
            {
                throw E_FILENAME_DUPLICATE;
            }
            filename = arg;
        }
        else if (arg[0] == '-')
        {
            if (arg[2] != '\0')
            {
                throw E_FLAG_UNDEFINE;
            }
            char flag = arg[1]; // \0 is also solved
            switch (flag)
            {
            case 'n':
                set_flag(flag_n);
                break;
            case 'w':
                set_flag(flag_w);
                break;
            case 'm':
                set_flag(flag_m);
                break;
            case 'c':
                set_flag(flag_c);
                break;
            case 'h':
                set_flag(flag_h);
                if (++i >= argc)
                {
                    throw E_FLAG_PARAM_NOT_EXIST;
                }
                parse_alpha(argv[i], param_h);
                break;
            case 't':
                set_flag(flag_t);
                if (++i >= argc)
                {
                    throw E_FLAG_PARAM_NOT_EXIST;
                }
                parse_alpha(argv[i], param_t);
                break;
            case 'r':
                set_flag(flag_r);
                break;
            default:
                throw E_FLAG_UNDEFINE;
            }
        }
        else
        {
            throw E_FLAG_UNDEFINE;
        }
    }
}

inline void read_file(char*& content, long& len)
{
    FILE* file;
    if (fopen_s(&file, filename, "rb") || file == nullptr)
    {
        throw E_FILE_UNABLE_TO_OPEN;
    }
    fseek(file, 0, SEEK_END);
    len = ftell(file);
    fseek(file, 0, SEEK_SET);
    content = (char*)malloc(static_cast<size_t>(len) + 1);
    if (content == nullptr)
    {
        throw E_MEMORY_ALLOC_FAIL;
    }
    size_t read = fread(content, 1, static_cast<size_t>(len) + 1, file);
    if (read != len)
    {
        throw E_FILE_UNABLE_TO_OPEN;
    }
    content[read] = 0;
    fclose(file);
}

int split_content(char* content, int len, char* result[]) 
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

inline void write_to_screen(char *result[], int len)
{
    printf("%d\n", len);
    for (int i = 0; i < len; i++)
    {
        puts(result[i]);
    }
}

inline void write_to_solution(char *result[], int len)
{
    FILE* file;
    if (fopen_s(&file, "solution.txt", "w") || file == nullptr)
    {
        throw E_FILE_UNABLE_TO_OPEN;
    }
    fprintf(file, "%d\n", len);
    for (int i = 0; i < len; i++)
    {
        fputs(result[i], file);
        fputc('\n', file);
    }
    fclose(file);
}

inline void call_core()
{
    long len;
    char* content;
    read_file(content, len);
    static char* words[10000];
    static char* result[20000];
    int word_count = split_content(content, len, words);
    if (flag_n)
    {
        int result_len = gen_chains_all(words, word_count, result);
        write_to_screen(result, result_len);
    }
    else if (flag_w)
    {
        int result_len = gen_chain_word(words, word_count, result, param_h, param_t, flag_r);
        write_to_solution(result, result_len);
    }
    else if (flag_m)
    {
        int result_len = gen_chain_word_unique(words, word_count, result);
        write_to_solution(result, result_len);
    }
    else if (flag_c)
    {
        int result_len = gen_chain_char(words, word_count, result, param_h, param_t, flag_r);
        write_to_solution(result, result_len);
    }
}

int main(int argc, char* argv[])
{
    printf("%d %d", add(1, 2), mul(1, 2));
    try
    {
        parse_params(argc, argv);
        conflict_check();
        call_core();
        /*
        printf("flag_n : %d\n", flag_n);
        printf("flag_w : %d\n", flag_w);
        printf("flag_m : %d\n", flag_m);
        printf("flag_c : %d\n", flag_c);
        printf("flag_h : %d\n", flag_h);
        printf("flag_t : %d\n", flag_t);
        printf("flag_r : %d\n", flag_r);
        */
    }
    catch (int e)
    {
        printf("error : %d\n", e);
    }
    return 0;
}
