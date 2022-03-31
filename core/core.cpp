#include "pch.h"
#include "core.h"
#include "node.h"
#include "chain.h"

#include <vector>
#include <cstring>
#include <algorithm>

using namespace std;

constexpr int WHITE = 0;
constexpr int GRAY = 1;
constexpr int BLACK = 2;

static int status[26];
static vector<Node> graph[26][26];

inline void build_graph(char* words[], const int word_count)
{
    for (int i = 0; i < 26; i++)
    {
        for (int j = 0; j < 26; j++)
        {
            graph[i][j].clear();
        }
    }
    for (int i = 0; i < word_count; i++)
    {
        char* word = words[i];
        int len = (int)strlen(word);
        graph[word[0] - 'a'][word[len - 1] - 'a'].emplace_back(len, word);
    }
    for (int i = 0; i < 26; i++)
    {
        for (int j = 0; j < 26; j++)
        {
            sort(graph[i][j].begin(), graph[i][j].end());
            graph[i][j].erase(unique(graph[i][j].begin(), graph[i][j].end()), graph[i][j].end());
        }
    }
}

bool check_cycle_dfs(const int now)
{
    status[now] = GRAY;
    for (int target = 0; target < 26; target++)
    {
        if (now == target)
        {
            continue;
        }
        if (!graph[now][target].empty())
        {
            if (status[target] == GRAY)
            {
                return true;
            }
            else if (status[target] == WHITE)
            {
                if (check_cycle_dfs(target))
                {
                    return true;
                }
            }
        }
    }
    status[now] = BLACK;
    return false;
}

bool check_cycle()
{
    memset(status, WHITE, sizeof(status));
    for (int i = 0; i < 26; i++)
    {
        if (status[i] == WHITE && check_cycle_dfs(i))
        {
            return true;
        }
        if (graph[i][i].size() > 1)
        {
            return true;
        }
    }
    return false;
}

inline void output_chain(const Chain& chain, Chain chains[], int& count)
{
    if (chain.get_word_len() > 1)
    {
        if (count < MAX_RESULT_LENGTH)
        {
            chains[count] = chain;
        }
        count++;
    }
}

inline void replace_max_chain(const Chain& chain, Chain& max_chain, const bool by_word)
{
    if (chain.get_word_len() > 1)
    {
        if (by_word && chain.get_word_len() > max_chain.get_word_len()) {
            max_chain = chain;
        }
        if (!by_word && chain.get_char_len() > max_chain.get_char_len()) {
            max_chain = chain;
        }
    }
}

void gen_chain_dfs(const int now, Chain& chain, Chain& max_chain, const char end, const bool by_word)
{
    for (int target = 0; target < 26; target++)
    {
        vector<Node>& nodes = graph[now][target];
        for (auto& node : nodes)
        {
            if (node.get_status())
            {
                continue;
            }
            node.set_status(1);
            chain.push_back(&node);
            if (end < 'a' || end > 'z' || node.get_end() == end)
            {
                replace_max_chain(chain, max_chain, by_word);
            }
            gen_chain_dfs(target, chain, max_chain, end, by_word);
            chain.pop_back();
            node.set_status(0);
            break;
        }
    }
}

int abstract_gen_chain(char* words[], const int len, char* result[], const char head, const char tail
    , const bool enable_loop, const bool by_word)
{
    build_graph(words, len);
    if (!enable_loop && check_cycle())
    {
        return E_WORD_CYCLE_EXIST;
    }
    Chain chain, max_chain;
    if ('a' <= head && head <= 'z')
    {
        gen_chain_dfs(head - 'a', chain, max_chain, tail, by_word);
    }
    else
    {
        for (int i = 0; i < 26; i++)
        {
            gen_chain_dfs(i, chain, max_chain, tail, by_word);
        }
    }
    for (int i = 0; i < max_chain.get_word_len(); i++)
    {
        result[i] = max_chain.at(i);
    }
    return max_chain.get_word_len();
}

void chains_all_dfs(const int now, Chain& chain, Chain chains[], int& count)
{
    Node* self_circle = nullptr;
    if (graph[now][now].size() == 1)
    {
        self_circle = &graph[now][now].at(0);
    }
    for (int target = 0; target < 26; target++)
    {
        if (now == target)
        {
            continue;
        }
        vector<Node>& nodes = graph[now][target];
        for (auto& node : nodes)
        {
            chain.push_back(&node);
            output_chain(chain, chains, count);
            chains_all_dfs(target, chain, chains, count);
            chain.pop_back();
            if (self_circle != nullptr)
            {
                chain.push_back(self_circle);
                output_chain(chain, chains, count);
                chain.push_back(&node);
                output_chain(chain, chains, count);
                chains_all_dfs(target, chain, chains, count);
                chain.pop_back();
                chain.pop_back();
            }
        }
    }
}

void chain_word_unique_dfs(const int now, Chain& chain, Chain& max_chain)
{
    for (int target = 0; target < 26; target++)
    {
        if (now == target)
        {
            continue;
        }
        vector<Node>& nodes = graph[now][target];
        for (auto& node : nodes)
        {
            chain.push_back(&node);
            replace_max_chain(chain, max_chain, true);
            chain_word_unique_dfs(target, chain, max_chain);
            if (graph[target][target].size() > 0)
            {
                chain.push_back(&graph[target][target].front());
                replace_max_chain(chain, max_chain, true);
                chain.pop_back();
            }
            chain.pop_back();
            break;
        }
    }
}

int gen_chain_word(char* words[], int len, char* result[], char head, char tail, bool enable_loop)
{
    return abstract_gen_chain(words, len, result, head, tail, enable_loop, true);
}

int gen_chains_all(char* words[], int len, char* result[])
{
    build_graph(words, len);
    if (check_cycle())
    {
        return E_WORD_CYCLE_EXIST;
    }
    Chain chain;
    int count = 0;
    static Chain chains[MAX_RESULT_LENGTH];
    for (int i = 0; i < 26; i++)
    {
        chains_all_dfs(i, chain, chains, count);
    }
    if (count >= MAX_RESULT_LENGTH)
    {
        return count;
    }
    for (int i = 0; i < count; i++)
    {
        result[i] = chains[i].to_string();
    }
    return count;
}

int gen_chain_word_unique(char* words[], int len, char* result[])
{
    build_graph(words, len);
    if (check_cycle())
    {
        return E_WORD_CYCLE_EXIST;
    }
    Chain chain, max_chain;
    for (int i = 0; i < 26; i++)
    {
        chain_word_unique_dfs(i, chain, max_chain);
    }
    for (int i = 0; i < max_chain.get_word_len(); i++)
    {
        result[i] = max_chain.at(i);
    }
    return max_chain.get_word_len();
}

int gen_chain_char(char* words[], int len, char* result[], char head, char tail, bool enable_loop)
{
    return abstract_gen_chain(words, len, result, head, tail, enable_loop, false);
}
