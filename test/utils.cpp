#include "pch.h"
#include "utils.h"

#include <io.h>
#include <string>
#include <vector>
#include <fstream>
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <unordered_set>

using namespace std;

bool check_testcase(const int index)
{
    string base = "../../cases/unit/testcase" + to_string(index);
    if (_access((base + ".in").c_str(), 0))
    {
        return false;
    }
    if (_access((base + ".config").c_str(), 0))
    {
        return false;
    }
    return true;
}

void read_by_line(const string name, char**& result, int& len)
{
    vector<char*> buffer;
    ifstream ifile(name.c_str());
    string line;
    while (getline(ifile, line))
    {
        if (!line.empty())
        {
            char* ptr = new char[line.length() + 1];
            strcpy_s(ptr, line.length() + 1, line.c_str());
            for (int i = 0; i < line.length(); i++)
            {
                ptr[i] += ('A' <= ptr[i] && ptr[i] <= 'Z') ? 'a' - 'A' : 0;
            }
            buffer.push_back(ptr);
        }
    }
    len = (int)buffer.size();
    result = new char*[len];
    for (int i = 0; i < len; i++)
    {
        result[i] = buffer[i];
    }
}

void free_content(char* data[], const int data_len)
{
    for (int i = 0; i < data_len; i++)
    {
        delete data[i];
    }
}

void parse_config(char* config[], const int config_len, Config& result)
{
    switch (config[0][0])
    {
    case 'n':
        result.process_method = ALL;
        break;
    case 'm':
        result.process_method = UNIQUE;
        break;
    case 'w':
        result.process_method = config[0][1] == 'r' ? BY_WORD_ALLOW_R : BY_WORD;
        break;
    case 'c':
        result.process_method = config[0][1] == 'r' ? BY_CHAR_ALLOW_R : BY_CHAR;
        break;
    }
    result.head = config[1][0];
    result.tail = config[2][0];
    switch (config[3][0])
    {
    case 's':
        result.check_method = BY_STRING;
        break;
    case 'w':
        result.check_method = BY_WORD_LEN;
        break;
    case 'c':
        result.check_method = BY_CHAR_LEN;
        break;
    }
    assert(sscanf_s(config[4], "%d", &result.answer) == 1);
}

inline void replace_all(char* data, const int data_len, const char src, const char dst)
{
    for (int i = 0; i < data_len; i++)
    {
        if (data[i] == src)
        {
            data[i] = dst;
        }
    }
}

inline bool array_unique_check(char* data[], const int data_len)
{
    unordered_set<char*> unique;
    for (int i = 0; i < data_len; i++)
    {
        if (unique.find(data[i]) != unique.end())
        {
            unique.insert(data[i]);
        }
        else
        {
            return false;
        }
    }
    return true;
}

inline bool string_unique_check(char data[], const int data_len)
{
    unordered_set<char*> unique;
    unique.insert(data);
    for (int i = 0; i < data_len; i++)
    {
        if (data[i] == 0)
        {
            if (unique.find(data + i + 1) != unique.end())
            {
                unique.insert(data + i + 1);
            }
            else
            {
                return false;
            }
        }
    }
    return true;
}

inline bool array_list_check(char* data[], const int data_len)
{
    if (strlen(data[0]) < 2)
    {
        return false;
    }
    for (int i = 1; i < data_len; i++)
    {
        int len = (int)strlen(data[i - 1]);
        if (len < 2 || data[i - 1][len - 1] != data[i][0])
        {
            return false;
        }
    }
    return true;
}

inline bool string_list_check(char* data, const int data_len)
{
    if (strlen(data) < 2)
    {
        return false;
    }
    for (int i = 0; i < data_len; i++)
    {
        if (data[i] == 0)
        {
            if (strlen(data + i + 1) < 2 || data[i + 1] != data[i - 1])
            {
                return false;
            }
        }
    }
    return true;
}

inline bool array_in_input_check(char* data[], const int data_len, char* input[], const int input_len)
{
    unordered_set<char*> input_set;
    for (int i = 0; i < input_len; i++)
    {
        input_set.insert(input[i]);
    }
    for (int i = 0; i < data_len; i++)
    {
        if (input_set.find(data[i]) == input_set.end())
        {
            return false;
        }
    }
    return true;
}

inline bool string_in_input_check(char* data, const int data_len, unordered_set<char*>& input)
{
    if (input.find(data) == input.end())
    {
        return false;
    }
    for (int i = 0; i < data_len; i++)
    {
        if (data[i] == 0)
        {
            if (input.find(data + i + 1) == input.end())
            {
                return false;
            }
        }
    }
    return true;
}

bool array_check_word(char* result[], const int result_len, char* input[], const int input_len, const int answer)
{
    if (result_len != answer)
    {
        return false;
    }
    if (result_len == 0)
    {
        return true;
    }
    return array_unique_check(result, result_len)
        && array_list_check(result, result_len)
        && array_in_input_check(result, result_len, input, input_len);
}

bool array_check_char(char* result[], const int result_len, char* input[], const int input_len, const int answer)
{
    int char_len = 0;
    for (int i = 0; i < result_len; i++)
    {
        char_len += (int)strlen(result[i]);
    }
    if (char_len != answer)
    {
        return false;
    }
    if (result_len == 0)
    {
        return true;
    }
    return array_unique_check(result, result_len)
        && array_list_check(result, result_len)
        && array_in_input_check(result, result_len, input, input_len);
}

bool string_check(char* result[], const int result_len, char* input[], const int input_len, const int answer)
{
    if (result_len != answer)
    {
        return false;
    }
    if (result_len == 0)
    {
        return true;
    }
    unordered_set<char*> input_set;
    for (int i = 0; i < input_len; i++)
    {
        input_set.insert(input[i]);
    }
    for (int i = 0; i < result_len; i++)
    {
        int len = (int)strlen(result[i]);
        replace_all(result[i], ' ', '\0', len);
        if (!string_unique_check(result[i], len)
            || !string_list_check(result[i], len)
            || !string_in_input_check(result[i], len, input_set))
        {
            replace_all(result[i], '\0', ' ', len);
            return false;
        }
        replace_all(result[i], '\0', ' ', len);
    }
    return true;
}
