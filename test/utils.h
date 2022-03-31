#pragma once

#include <string>

using namespace std;

constexpr int ALL = 0;
constexpr int UNIQUE = 1;
constexpr int BY_WORD = 2;
constexpr int BY_WORD_ALLOW_R = 3;
constexpr int BY_CHAR = 4;
constexpr int BY_CHAR_ALLOW_R = 5;

struct Config
{
	int method;
	char head, tail;
	bool sorted_check;
};

bool check_testcase(const int index);

void read_by_line(const string name, char**& result, int& len);

Config parse_config(char* config[], const int config_len);

bool content_compare(char* answer[], const int answer_len, char* result[], const int result_len);

bool sorted_compare(char* answer[], const int answer_len, char* result[], const int result_len);
