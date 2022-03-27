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
	char last;
	int status;

	Node(int len, char* word) : len(len), word(word), status(0) {
		last = word[len - 1];
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
		char *word = words[i];
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

void chains_all_dfs(int now, // status (character)
	string& chain,
	int chain_word_size, 
	char* result[], 
	int& count)
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
		else
		{
		vector<Node>& nodes = graph[now][target];
			for (auto& node : nodes)
			{
				chain[chain_char_size+1] = ' ';
				strcpy(chain + 1 + chain_char_size, node.word);
				chains_all_dfs(node.last, chain, chain_char_size + node.len + 1, wor)
				
				chain[chain_char_size] = 0;
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
	static char chain[20000] = {0};
	// TODO 20000
	for (int i = 0; i < 26; i++)
	{
		chains_all_dfs(i, chain, 0, 0, result, count);
	}
	return count;
}

int gen_chain_word_unique(char* words[], int len, char* result[])
{
	build(words, len, false);
	if (check_cycle())
	{
		throw E_WORD_CYCLE_EXIST;
	}
	return 0;
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
