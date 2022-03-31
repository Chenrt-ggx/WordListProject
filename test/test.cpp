﻿#include "pch.h"
#include "utils.h"
#include "CppUnitTest.h"

#include <cassert>

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
		TEST_METHOD(TestMethod)
		{
			int in_len;
			int config_len;
			char** in_content;
			char** config_content;
			int result_len;
			static char* result[20000];
			for (int i = 0; check_testcase(i + 1); i++)
			{
				string base = "../cases/unit/testcase" + i;
				read_by_line(base + ".in", in_content, in_len);
				read_by_line(base + ".config", config_content, config_len);
				Config config = parse_config(config_content, config_len);
				switch (config.process_method)
				{
				case ALL:
					result_len = gen_chains_all(in_content, in_len, result);
					break;
				case UNIQUE:
					result_len = gen_chain_word_unique(in_content, in_len, result);
					break;
				case BY_WORD:
					result_len = gen_chain_word(in_content, in_len, result, config.head, config.tail, false);
					break;
				case BY_WORD_ALLOW_R:
					result_len = gen_chain_word(in_content, in_len, result, config.head, config.tail, false);
					break;
				case BY_CHAR:
					result_len = gen_chain_char(in_content, in_len, result, config.head, config.tail, true);
					break;
				case BY_CHAR_ALLOW_R:
					result_len = gen_chain_char(in_content, in_len, result, config.head, config.tail, true);
					break;
				default:
					result_len = -1;
				}
				switch (config.check_method)
				{
				case BY_STRING:
					assert(string_check(result, result_len, in_content, in_len, config.answer));
					break;
				case BY_WORD_LEN:
					assert(array_check_word(result, result_len, in_content, in_len, config.answer));
					break;
				case BY_CHAR_LEN:
					assert(array_check_char(result, result_len, in_content, in_len, config.answer));
					break;
				}
				free_content(result, result_len);
				free_content(in_content, in_len);
				free_content(config_content, config_len);
				delete in_content;
				delete config_content;
			}
		}
	};
}
