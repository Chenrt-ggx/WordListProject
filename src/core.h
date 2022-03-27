#pragma once

#include <vector>

using namespace std;

int gen_chain_word(char* words[], int len, char* result[], char head, char tail, bool enable_loop);

int gen_chains_all(char* words[], int len, char* result[]);

int gen_chain_word_unique(char* words[], int len, char* result[]);

int gen_chain_char(char* words[], int len, char* result[], char head, char tail, bool enable_loop);

static void build(char* words[], int word_count, bool enable_loop);

bool check_cycle();

bool check_cycle_dfs(int parent);
