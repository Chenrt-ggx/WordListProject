#include "utils.h"
#include "errors.h"

#include <cstdio>
#include <cassert>
#include <cstdlib>

using namespace std;

#ifdef _DEBUG
    #pragma comment(lib, "../x64/Debug/core.lib")
#else
    #pragma comment(lib, "../x64/Release/core.lib")
#endif

extern "C" _declspec(dllimport) int gen_chain_word(char* words[], int len, char* result[], char head, char tail, bool enable_loop);

extern "C" _declspec(dllimport) int gen_chains_all(char* words[], int len, char* result[]);

extern "C" _declspec(dllimport) int gen_chain_word_unique(char* words[], int len, char* result[]);

extern "C" _declspec(dllimport) int gen_chain_char(char* words[], int len, char* result[], char head, char tail, bool enable_loop);

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

void parse_params(int argc, char* argv[]) 
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
            if (arg[1] == '\0' || arg[2] != '\0')
            {
                throw E_FLAG_UNDEFINE;
            }
            switch (arg[1])
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

void conflict_check()
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

int main(int argc, char* argv[])
{
    int len;
    int return_value = 0;
    char* content = nullptr;
    static char* words[10005];
    static char* result[20005];
    try
    {
        parse_params(argc, argv);
        conflict_check();
        read_file(filename, content, len);
        int word_count = split_content(content, len, words);
        if (flag_n)
        {
            int result_len = gen_chains_all(words, word_count, result);
            if (result_len < 0)
            {
                throw result_len;
            }
            write_to_screen(result, result_len);
        }
        else if (flag_w)
        {
            int result_len = gen_chain_word(words, word_count, result, param_h, param_t, flag_r);
            if (result_len < 0)
            {
                throw result_len;
            }
            write_to_solution(result, result_len);
        }
        else if (flag_m)
        {
            int result_len = gen_chain_word_unique(words, word_count, result);
            if (result_len < 0)
            {
                throw result_len;
            }
            write_to_solution(result, result_len);
        }
        else if (flag_c)
        {
            int result_len = gen_chain_char(words, word_count, result, param_h, param_t, flag_r);
            if (result_len < 0)
            {
                throw result_len;
            }
            write_to_solution(result, result_len);
        }
    }
    catch (int eid)
    {
        const char* description[] =
        {
            "unexpected word cycle detected", // 1
            "undefined flag detected", // 2
            "flag conflict", // 3
            "required flag not exist", // 4
            "wrong flag param format", // 5
            "required flag param not exist", // 6
            "required filename not exist", // 7
            "duplicated filename", // 8
            "input file not exist", // 9
            "input file unable to open", // A
            "input file modified when reading", // B
            "memory alloc failed", // C
        };
        int index = ((eid ^ 0x80000000) - 1) % (sizeof(description) / sizeof(char*));
        printf("expection (id 0x%X) catched: %s\n", eid, description[index]);
        return_value = eid;
    }
    if (content != nullptr)
    {
        free(content);
    }
    return return_value;
}
