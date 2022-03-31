#include "pch.h"

#include <vector>
#include <string>
#include <cstring>
#include <algorithm>

using namespace std;

vector<char*> answer_reader(const string name)
{
	vector<char*> answer;
	return answer;
}

bool diff_compare(const vector<char*>& answer, int len, char* result[])
{
	if (len != answer.size())
	{
		return false;
	}
	for (int i = 0; i < len; i++)
	{
		if (strcmp(answer[i], result[i]))
		{
			return false;
		}
	}
	return true;
}

bool sort_compare(const vector<char*>& answer, int len, char* result[])
{
	sort(result, result + len);
	return diff_compare(answer, len, result);
}
