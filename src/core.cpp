#include "core.h"
#include "global.h"

#include <vector>
#include <cstring>
#include <algorithm>
#include <unordered_set>

using namespace std;

constexpr int WHITE = 0;
constexpr int GRAY = 1;
constexpr int BLACK = 2;

struct Node
{
	int len;
	char* word;
	int status;

	Node(int len, char* word) : len(len), word(word), status(0) {
	};

	bool operator==(Node& rhs) const
	{
		return strcmp(this->word, rhs.word) == 0;
	}

	bool operator>(Node& rhs) const
	{
		if (this->len < rhs.len)
		{
			return true;
		}
		if (this->len > rhs.len)
		{
			return false;
		}
		return strcmp(this->word, rhs.word) < 0;
	}

	bool operator<(Node& rhs) const
	{
		if (this->len > rhs.len)
		{
			return true;
		}
		if (this->len < rhs.len)
		{
			return false;
		}
		return strcmp(this->word, rhs.word) > 0;
	}
};

vector<Node> graph[26][26];

static void reset_graph()
{
	for (int i = 0; i < 26; i++)
	{
		for (int j = 0; j < 26; j++)
		{
			for (Node& node : graph[i][j]) {
				node.status = WHITE;
			}
		}
	}
}

static void build(char* words[], int word_count, bool enable_loop)
{
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
			if (enable_loop)
			{
				// check_loop and remove_duplicates are of xor relations.
				graph[i][j].erase(unique(graph[i][j].begin(), graph[i][j].end()), graph[i][j].end());
			}
		}
	}
	//for (int i = 0; i < 26; i++)
	//{
	//	for (int j = 0; j < 26; j++)
	//	{
	//		printf("(%c, %c): ", i + 'a', j + 'a');
	//		for (Node i : graph[i][j])
	//		{
	//			printf("%s ", i.word);
	//		}
	//		puts("");
	//	}
	//}
}

int status[26];

bool check_cycle_dfs(int parent)
{
	for (int i = 0; i < 26; i++)
	{
		if (parent == i)
		{
			continue;
		}
		if (!graph[parent][i].empty())
		{
			if (status[i] == WHITE)
			{
				status[i] = GRAY;
				if (check_cycle_dfs(i))
				{
					return true;
				}
				status[i] = BLACK;
			}
			else if (status[i] == GRAY)
			{
				return true;
			}
		}
	}
	return false;
}

bool check_cycle() 
{
	memset(status, 0, sizeof(status));
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

int gen_chain_word(char* words[], int len, char* result[], char head, char tail, bool enable_loop)
{
	build(words, len, enable_loop);
	if (!enable_loop && check_cycle())
	{
		throw E_WORD_CYCLE_EXIST;
	}
	return 0;
}

inline void __output(vector<char*>& chain, vector<char*> chains[], int& count) 
{
	if (chain.size() > 1)
	{
		if (count < MAX_RESULT_LENGTH)
		{
			chains[count] = chain;
		}
		++count;
	}
}

void chains_all_dfs(int now, vector<char*>& chain, vector<char*> chains[], int& count)
{
	Node* self_circle = nullptr;
	if (graph[now][now].size() == 1)
	{
		// there is a self circle
		printf("sure");
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
			chain.push_back(node.word);
			__output(chain, chains, count);
			chains_all_dfs(target, chain, chains, count);
			chain.pop_back();
			if (self_circle != nullptr)
			{
				chain.push_back(self_circle->word);
				__output(chain, chains, count);
				chain.push_back(node.word);
				__output(chain, chains, count);
				printf("yes");
				chains_all_dfs(target, chain, chains, count);
				chain.pop_back();
				chain.pop_back();
			}
		}
	}
}

inline char* to_string(const vector<char*>& chain)
{
	string str;
	for (auto& i : chain)
	{
		str += i;
		str += " ";
	}
	char* result = (char*)malloc(str.length() + 1);
	if (result == nullptr)
	{
		throw E_MEMORY_ALLOC_FAIL;
	}
	strcpy_s(result, str.length() + 1, str.c_str());
	return result;
}

int gen_chains_all(char* words[], int len, char* result[])
{
	build(words, len, false);
	if (check_cycle()) 
	{
		throw E_WORD_CYCLE_EXIST;
	}
	int count = 0;
	vector<char*> chain;
	static vector<char*> chains[20000];
	for (int i = 0; i < 26; i++)
	{
		chains_all_dfs(i, chain, chains, count);
	}
	if (count >= 20000)
	{
		return count;
	}
	for (int i = 0; i < count; i++)
	{
		result[i] = to_string(chains[i]);
	}
	return count;
}

inline void __add_max_chain(vector<char*>& chain, vector<char*>& max_chain)
{
	if (chain.size() > 1)
	{
		if (chain.size() > max_chain.size()) {
			max_chain = chain;
		}
	}
}

void __chain_word_unique_dfs(int now, vector<char*> &chain, vector<char*>& max_chain)
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
			chain.push_back(node.word);
			__add_max_chain(chain, max_chain);
			__chain_word_unique_dfs(target, chain, max_chain);
			if (graph[target][target].size() > 0)
			{
				// the last word of a chain could be axxa
				chain.push_back(graph[target][target].front().word);
				__add_max_chain(chain, max_chain);
				chain.pop_back();
			}
			chain.pop_back();
			break;
		}
	}
}

int gen_chain_word_unique(char* words[], int len, char* result[])
{
	build(words, len, false);
	if (check_cycle())
	{
		throw E_WORD_CYCLE_EXIST;
	}
	vector<char*> chain;
	vector<char*> max_chain;
	for (int i = 0; i < 26; i++)
	{
		__chain_word_unique_dfs(i, chain, max_chain);
	}

	for (int i = 0; i < max_chain.size(); i++)
	{
		result[i] = max_chain[i];
	}

	return max_chain.size();
}

int gen_chain_char(char* words[], int len, char* result[], char head, char tail, bool enable_loop)
{
	build(words, len, enable_loop);
	if (!enable_loop && check_cycle())
	{
		throw E_WORD_CYCLE_EXIST;
	}
	return 0;
}
