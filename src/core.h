#pragma once

#include <vector>

using namespace std;

vector<char*> gen_chain_word(vector<char*> words, char head, char tail, bool enable_loop);

vector<char*> gen_chains_all(vector<char*> words);

vector<char*> gen_chain_word_unique(vector<char*> words);

vector<char*> gen_chain_char(vector<char*> words, char head, char tail, bool enable_loop);

static void build(vector<char*> words, bool enable_loop);

void check_cycle();

bool check_dfs(int parent);
