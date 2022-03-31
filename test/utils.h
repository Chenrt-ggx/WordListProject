#pragma once

#include <string>

using namespace std;

constexpr int ALL = 0;
constexpr int UNIQUE = 1;
constexpr int BY_WORD = 2;
constexpr int BY_WORD_ALLOW_R = 3;
constexpr int BY_CHAR = 4;
constexpr int BY_CHAR_ALLOW_R = 5;

constexpr int BY_STRING = 0;
constexpr int BY_WORD_LEN = 1;
constexpr int BY_CHAR_LEN = 2;

struct Config
{
    int answer;
    char head, tail;
    int process_method, check_method;
};

bool check_testcase(const int index);

void read_by_line(const string name, char**& result, int& len);

void free_content(char* data[], const int data_len);

void parse_config(char* config[], const int config_len, Config& result);

bool array_check_word(char* result[], const int result_len, char* input[], const int input_len, const int answer);

bool array_check_char(char* result[], const int result_len, char* input[], const int input_len, const int answer);

bool string_check(char* result[], const int result_len, char* input[], const int input_len, const int answer);
