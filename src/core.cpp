#include "core.h"

#include <vector>
#include <cstring>
#include <algorithm>
#include <unordered_set>

using namespace std;

struct Node
{
	int len;
	char* word;

	Node(int len, char* word) : len(len), word(word) {};

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

static void build(vector<char*> words)
{
	for (char* i : words)
	{
		int len = (int)strlen(i);
		graph[i[0] - 'a'][i[len - 1] - 'a'].emplace_back(len, i);
	}
	for (int i = 0; i < 26; i++)
	{
		for (int j = 0; j < 26; j++)
		{
			sort(graph[i][j].begin(), graph[i][j].end());
			graph[i][j].erase(unique(graph[i][j].begin(), graph[i][j].end()), graph[i][j].end());
		}
	}
	for (int i = 0; i < 26; i++)
	{
		for (int j = 0; j < 26; j++)
		{
			printf("(%c, %c): ", i + 'a', j + 'a');
			for (Node i : graph[i][j])
			{
				printf("%s ", i.word);
			}
			puts("");
		}
	}
}

vector<char*> gen_chain_word(vector<char*> words, char head, char tail, bool enable_loop)
{
	build(words);
	return vector<char*>(1);
}

vector<char*> gen_chains_all(vector<char*> words)
{
	build(words);
	return vector<char*>(1);
}

vector<char*> gen_chain_word_unique(vector<char*> words)
{
	build(words);
	return vector<char*>(1);
}

vector<char*> gen_chain_char(vector<char*> words, char head, char tail, bool enable_loop)
{
	build(words);
	return vector<char*>(1);
}
