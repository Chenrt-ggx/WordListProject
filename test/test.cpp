#include "pch.h"
#include "CppUnitTest.h"

#pragma comment(lib, "../x64/Debug/core.lib")

extern "C" _declspec(dllimport) int gen_chain_word(char* words[], int len, char* result[], char head, char tail, bool enable_loop);

extern "C" _declspec(dllimport) int gen_chains_all(char* words[], int len, char* result[]);

extern "C" _declspec(dllimport) int gen_chain_word_unique(char* words[], int len, char* result[]);

extern "C" _declspec(dllimport) int gen_chain_char(char* words[], int len, char* result[], char head, char tail, bool enable_loop);

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace test
{
	TEST_CLASS(test)
	{
	public:
		TEST_METHOD(TestMethod1)
		{
			char* words[] = { "aaa", "aba", "aca" };
			static char* result[20000];
			int tot = gen_chains_all(words, 3, result);
			for (int i = 0; i < tot; i++)
			{
				puts(result[i]);
			}
		}
	};
}
