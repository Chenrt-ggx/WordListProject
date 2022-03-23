#include <cstdio>
#include <cctype>
#include <cstring>
#include <cstdlib>
#include "global.h"

bool flag_n = false;
bool flag_w = false;
bool flag_m = false;
bool flag_c = false;
bool flag_h = false;
bool flag_t = false;
bool flag_r = false;

char param_h;
char param_t;

char* filename = nullptr;

inline bool is_txt(char* s)
{
    int len = strlen(s);
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
        throw E_FILENAME_NOT_PROVID;
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
                flag_h = true;
                if (++i >= argc)
                {
                    throw E_FLAG_PARAM_NOT_EXIST;
                }
                parse_alpha(argv[i], param_h);
                break;
            case 't':
                flag_t = true;
                if (++i >= argc)
                {
                    throw E_FLAG_PARAM_NOT_EXIST;
                }
                parse_alpha(argv[i], param_t);
                break;
            case 'r':
                flag_r = true;
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

int main(int argc, char* argv[])
{
    try
    {
        parse_params(argc, argv);
        conflict_check();
        printf("flag_n : %d\n", flag_n);
        printf("flag_w : %d\n", flag_w);
        printf("flag_m : %d\n", flag_m);
        printf("flag_c : %d\n", flag_c);
        printf("flag_h : %d\n", flag_h);
        printf("flag_t : %d\n", flag_t);
        printf("flag_r : %d\n", flag_r);
    }
    catch (int e)
    {
        printf("error : %d\n", e);
    }
    return 0;
}
