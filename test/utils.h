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
constexpr int BY_UNIQUE = 1;
constexpr int BY_WORD_LEN = 2;
constexpr int BY_CHAR_LEN = 3;
constexpr int HAS_ERROR = 4;

struct Config
{
    int answer;
    char head, tail;
    int process_method, check_method;
};

void check_testcase(const int index, const string& folder);

void create_temp_file(const string& name, char* data[], const int data_len);

void remove_temp_file(const string& name);

void read_by_line(const string& name, char**& result, int& len);

void free_content(char* data[], const int data_len);

void parse_config(char* config[], const int config_len, Config& result);

void array_check_word(char* result[], const int result_len, char* input[], const int input_len, const Config& config);

void array_check_char(char* result[], const int result_len, char* input[], const int input_len, const Config& config);

void unique_check(char* result[], const int result_len, char* input[], const int input_len, const Config& config);

void string_check(char* result[], const int result_len, char* input[], const int input_len, const Config& config);

void error_code_check(const int result_len, const Config& config);

void cli_parse_check(const int result, char* data[], const int data_len);
