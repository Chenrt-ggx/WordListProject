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
	int end;
	int status;
	char* word;

	Node(int len, char* word) : len(len), word(word), status(0) {
		end = word[len - 1];
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

struct Chain
{
	int char_len;
	vector<Node*> word_vector;

	Chain()
	{
		char_len = 0;
	}

	inline char* at(int index)
	{
		return word_vector[index]->word;
	}

	inline int get_word_len()
	{
		return (int)word_vector.size();
	}

	inline int get_char_len()
	{
		return char_len;
	}

	inline void push_back(Node* word)
	{
		word_vector.push_back(word);
		char_len += word->len;
	}

	inline void pop_back()
	{
		char_len -= word_vector.back()->len;
		word_vector.pop_back();
	}

	inline char* to_string()
	{
		string str;
		for (auto& i : word_vector)
		{
			str += i->word;
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

inline void __add_max_chain(Chain& chain, Chain& max_chain, bool by_word)
{
	if (chain.get_word_len() > 1)
	{
		if (by_word && chain.get_word_len() > max_chain.get_word_len()) {
			max_chain = chain;
		}
		else if (!by_word && chain.get_char_len() > max_chain.get_char_len()) {
			max_chain = chain;
		}
	}
}

void __chain_word_dfs(int now, Chain& chain, Chain& max_chain, char end, bool by_word)
{
	for (int target = 0; target < 26; target++)
	{
		vector<Node>& nodes = graph[now][target];
		for (auto& node : nodes)
		{
			if (node.status)
			{
				continue;
			}
			node.status = 1;
			chain.push_back(&node);
			if (end < 'a' || end > 'z' || node.end == end)
			{
				__add_max_chain(chain, max_chain, by_word);
			}
			__chain_word_dfs(target, chain, max_chain, end, by_word);
			chain.pop_back();
			node.status = 0;
			break;
		}
	}
}

int abstract_gen_chain(char* words[], int len, char* result[], char head, char tail, bool enable_loop, bool by_word)
{
	build(words, len, enable_loop);
	if (!enable_loop && check_cycle())
	{
		throw E_WORD_CYCLE_EXIST;
	}
	Chain chain;
	Chain max_chain;
	if ('a' <= head && head <= 'z')
	{
		__chain_word_dfs(head - 'a', chain, max_chain, tail, by_word);
	}
	else
	{
		for (int i = 0; i < 26; i++)
		{
			__chain_word_dfs(i, chain, max_chain, tail, by_word);
		}
	}
	for (int i = 0; i < max_chain.get_word_len(); i++)
	{
		result[i] = max_chain.at(i);
	}

	return (int)max_chain.get_word_len();
}

int gen_chain_word(char* words[], int len, char* result[], char head, char tail, bool enable_loop)
{
	return abstract_gen_chain(words, len, result, head, tail, enable_loop, true);
}

inline void __output(Chain& chain, Chain chains[], int& count)
{
	if (chain.get_word_len() > 1)
	{
		if (count < MAX_RESULT_LENGTH)
		{
			chains[count] = chain;
		}
		++count;
	}
}

void chains_all_dfs(int now, Chain& chain, Chain chains[], int& count)
{
	Node* self_circle = nullptr;
	if (graph[now][now].size() == 1)
	{
		// there is a self circle
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
			__output(chain, chains, count);
			chains_all_dfs(target, chain, chains, count);
			chain.pop_back();
			if (self_circle != nullptr)
			{
				chain.push_back(self_circle);
				__output(chain, chains, count);
				chain.push_back(&node);
				__output(chain, chains, count);
				chains_all_dfs(target, chain, chains, count);
				chain.pop_back();
				chain.pop_back();
			}
		}
	}
}

int gen_chains_all(char* words[], int len, char* result[])
{
	build(words, len, false);
	if (check_cycle()) 
	{
		throw E_WORD_CYCLE_EXIST;
	}
	int count = 0;
	Chain chain;
	static Chain chains[20000];
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
		result[i] = chains[i].to_string();
	}
	return count;
}

void __chain_word_unique_dfs(int now, Chain& chain, Chain& max_chain)
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
			__add_max_chain(chain, max_chain, true);
			__chain_word_unique_dfs(target, chain, max_chain);
			if (graph[target][target].size() > 0)
			{
				// the last word of a chain could be axxa
				chain.push_back(&graph[target][target].front());
				__add_max_chain(chain, max_chain, true);
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
	Chain chain;
	Chain max_chain;
	for (int i = 0; i < 26; i++)
	{
		__chain_word_unique_dfs(i, chain, max_chain);
	}

	for (int i = 0; i < max_chain.get_word_len(); i++)
	{
		result[i] = max_chain.at(i);
	}

	return (int)max_chain.get_word_len();
}

int gen_chain_char(char* words[], int len, char* result[], char head, char tail, bool enable_loop)
{
	return abstract_gen_chain(words, len, result, head, tail, enable_loop, false);
}
