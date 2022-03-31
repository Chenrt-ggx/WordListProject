#pragma once

#include <vector>
#include <string>

using namespace std;

vector<char*> answer_reader(const string name);

bool diff_compare(const vector<char*>& answer, int len, char* result[]);

bool sort_compare(const vector<char*>& answer, int len, char* result[]);
