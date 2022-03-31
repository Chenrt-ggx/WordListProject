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

using namespace std;

bool check_testcase(const int index)
{
	string base = "../cases/unit/testcase" + index;
	if (_access((base + ".in").c_str(), 0))
	{
		return false;
	}
	if (_access((base + ".out").c_str(), 0))
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

Config parse_config(char* config[], const int config_len)
{
	Config result;
	switch (config[0][0])
	{
	case 'n':
		result.method = ALL;
		break;
	case 'm':
		result.method = UNIQUE;
		break;
	case 'w':
		result.method = config[0][1] == 'r' ? BY_WORD_ALLOW_R : BY_WORD;
		break;
	case 'c':
		result.method = config[0][1] == 'r' ? BY_CHAR_ALLOW_R : BY_CHAR;
		break;
	}
	result.head = config[1][0];
	result.tail = config[2][0];
	result.sorted_check = config[3][0] ^ 48;
	return result;
}

bool content_compare(char* answer[], const int answer_len, char* result[], const int result_len)
{
	if (result_len != answer_len)
	{
		return false;
	}
	for (int i = 0; i < result_len; i++)
	{
		if (strcmp(answer[i], result[i]))
		{
			return false;
		}
	}
	return true;
}

bool sorted_compare(char* answer[], const int answer_len, char* result[], const int result_len)
{
	sort(answer, answer + answer_len);
	sort(result, result + result_len);
	return content_compare(answer, answer_len, result, result_len);
}
