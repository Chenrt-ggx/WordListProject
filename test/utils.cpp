#include "pch.h"
#include "utils.h"
#include "CppUnitTest.h"

#include <io.h>
#include <string>
#include <vector>
#include <fstream>
#include <cstring>
#include <unordered_set>

using namespace std;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

void check_testcase(const int index)
{
    string base = "../../cases/unit/testcase" + to_string(index);
    Assert::AreEqual(_access((base + ".in").c_str(), 0), 0);
    Assert::AreEqual(_access((base + ".config").c_str(), 0), 0);
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
        result.check_method = BY_STRING;
        break;
    case 'm':
        result.process_method = UNIQUE;
        result.check_method = BY_WORD_LEN;
        break;
    case 'w':
        result.process_method = config[0][1] == 'r' ? BY_WORD_ALLOW_R : BY_WORD;
        result.check_method = BY_WORD_LEN;
        break;
    case 'c':
        result.process_method = config[0][1] == 'r' ? BY_CHAR_ALLOW_R : BY_CHAR;
        result.check_method = BY_CHAR_LEN;
        break;
    }
    result.head = config[1][0];
    result.tail = config[2][0];
    Assert::AreEqual(sscanf_s(config[3], "%d", &result.answer), 1);
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

inline void array_unique_check(char* data[], const int data_len)
{
    unordered_set<string> unique;
    for (int i = 0; i < data_len; i++)
    {
        Assert::IsTrue(unique.find(data[i]) == unique.end());
        unique.insert(data[i]);
    }
}

inline void string_unique_check(char data[], const int data_len)
{
    unordered_set<string> unique;
    unique.insert(data);
    for (int i = 0; i < data_len; i++)
    {
        if (data[i] == 0)
        {
            Assert::IsTrue(unique.find(data + i + 1) == unique.end());
            unique.insert(data + i + 1);
        }
    }
}

inline void array_list_check(char* data[], const int data_len)
{
    Assert::IsTrue(strlen(data[0]) >= 2);
    for (int i = 1; i < data_len; i++)
    {
        int len = (int)strlen(data[i - 1]);
        Assert::IsTrue(len >= 2);
        Assert::AreEqual(data[i - 1][len - 1], data[i][0]);
    }
}

inline void string_list_check(char* data, const int data_len)
{
    Assert::IsTrue(strlen(data) >= 2);
    for (int i = 0; i < data_len; i++)
    {
        if (data[i] == 0)
        {
            Assert::IsTrue(strlen(data + i + 1) >= 2);
            Assert::AreEqual(data[i + 1], data[i - 1]);
        }
    }
}

inline void array_in_input_check(char* data[], const int data_len, char* input[], const int input_len)
{
    unordered_set<string> input_set;
    for (int i = 0; i < input_len; i++)
    {
        input_set.insert(input[i]);
    }
    for (int i = 0; i < data_len; i++)
    {
        Assert::IsFalse(input_set.find(data[i]) == input_set.end());
    }
}

inline void string_in_input_check(char* data, const int data_len, unordered_set<string>& input)
{
    Assert::IsFalse(input.find(data) == input.end());
    for (int i = 0; i < data_len; i++)
    {
        if (data[i] == 0)
        {
            Assert::IsFalse(input.find(data + i + 1) == input.end());
        }
    }
}

void array_check_word(char* result[], const int result_len, char* input[], const int input_len, const int answer)
{
    Assert::AreEqual(result_len, answer);
    if (result_len != 0)
    {
        array_unique_check(result, result_len);
        array_list_check(result, result_len);
        array_in_input_check(result, result_len, input, input_len);
    }
}

void array_check_char(char* result[], const int result_len, char* input[], const int input_len, const int answer)
{
    int char_len = 0;
    for (int i = 0; i < result_len; i++)
    {
        char_len += (int)strlen(result[i]);
    }
    Assert::AreEqual(char_len, answer);
    if (result_len != 0)
    {
        array_unique_check(result, result_len);
        array_list_check(result, result_len);
        array_in_input_check(result, result_len, input, input_len);
    }
}

void string_check(char* result[], const int result_len, char* input[], const int input_len, const int answer)
{
    Assert::AreEqual(result_len, answer);
    if (result_len != 0)
    {
        unordered_set<string> input_set;
        for (int i = 0; i < input_len; i++)
        {
            input_set.insert(input[i]);
        }
        for (int i = 0; i < result_len; i++)
        {
            int len = (int)strlen(result[i]);
            replace_all(result[i], len, ' ', '\0');
            string_unique_check(result[i], len);
            string_list_check(result[i], len);
            string_in_input_check(result[i], len, input_set);
            replace_all(result[i], len, '\0', ' ');
        }
    }
}

void error_code_check(const int result_len, const int answer)
{
    Assert::AreEqual((int)(result_len ^ 0x80000000), answer);
}
