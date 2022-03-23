#include <cstdio>
#include <cstring>
#include <cctype>
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
    if (len < 4)
    {
        return false;
    }
    else
    {
        return strstr(s + len - 4, ".txt") != nullptr;
    }
}

inline int parse_alpha(char* s)
{
    if (strlen(argv[i]) == 1)
    {
        char c = *(argv[i]);
        if (isalpha(c))
        {
            param_h = tolower(c);
        }
        else
        {
            return E_FLAG_PARAM_FORMAT_ERROR;
        }
    }
    else
    {
        return E_FLAG_PARAM_FORMAT_ERROR;
    }
}

inline int parse_params(int argc, char* argv[]) 
{
    for (int i = 1; i < argc; i++)
    {
        char* arg = argv[i];
        if (is_txt(arg)) 
        {
            if (filename != nullptr)
            {
                return E_FLAG_CONFLICT;
            }
            filename = arg;
        }
        else if (arg[0] == '-')
        {
            char flag = arg[1]; // \0 is also solved
            switch (flag)
            {
            case 'n':
                flag_n = true;
                break;
            case 'w':
                flag_w = true;
                break;
            case 'm':
                flag_m = true;
                break;
            case 'c':
                flag_c = true;
                break;
            case 'h':
                flag_h = true;
                ++i;
                if (i < argc)
                {

                }
                else
                {
                    return E_FLAG_PARAM_NOT_EXISTS;
                }
                break;
            case 't':
                flag_t = true;

                break;
            case 'r':
                flag_r = true;
                break;
            default:
                return E_FLAG_NOT_DEFINED;
            }
        }
        else
        {
            return E_FLAG_NOT_DEFINED;
        }
    }
}

int main(int argc, char* argv[])
{
    

    

}

