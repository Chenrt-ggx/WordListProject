#include "pch.h"
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
    private:
        Config config;
        char** in_content;
        char** config_content;
        int in_len, config_len, result_len;

        void do_test_at(const int index)
        {
            static char* result[20000];
            Assert::AreEqual(check_testcase(index), true);
            string base = "../../cases/unit/testcase" + to_string(index);
            read_by_line(base + ".in", in_content, in_len);
            read_by_line(base + ".config", config_content, config_len);
            parse_config(config_content, config_len, config);
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
                result_len = gen_chain_word(in_content, in_len, result, config.head, config.tail, true);
                break;
            case BY_CHAR:
                result_len = gen_chain_char(in_content, in_len, result, config.head, config.tail, false);
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
                Assert::AreEqual(string_check(result, result_len, in_content, in_len, config.answer), true);
                break;
            case BY_WORD_LEN:
                Assert::AreEqual(array_check_word(result, result_len, in_content, in_len, config.answer), true);
                break;
            case BY_CHAR_LEN:
                Assert::AreEqual(array_check_char(result, result_len, in_content, in_len, config.answer), true);
                break;
            }
            free_content(in_content, in_len);
            free_content(config_content, config_len);
            delete in_content;
            delete config_content;
        }
    public:
        TEST_METHOD(AllChainNormal)
        {
            do_test_at(1);
        }

        TEST_METHOD(ByWordNormal)
        {
            do_test_at(2);
        }

        TEST_METHOD(UniqueChainNormal)
        {
            do_test_at(3);
        }

        TEST_METHOD(ByCharNormal)
        {
            do_test_at(4);
        }

        TEST_METHOD(ByWordAllowCycleNormal)
        {
            do_test_at(5);
        }

        TEST_METHOD(ByCharAllowCycleNormal)
        {
            do_test_at(6);
        }
    };
}
