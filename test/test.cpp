#include "pch.h"
#include "utils.h"
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
    private:
        Config config;
        char** in_content;
        char** config_content;
        int in_len, config_len, result_len;

        void do_test_at(const int index)
        {
            static char* result[20000];
            check_testcase(index);
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
            }
            if (result_len < 0)
            {
                error_code_check(result_len, config);
            }
            else if (config.check_method == BY_STRING)
            {
                string_check(result, result_len, in_content, in_len, config);
            }
            else if (config.check_method == BY_UNIQUE)
            {
                unique_check(result, result_len, in_content, in_len, config);
            }
            else if (config.check_method == BY_WORD_LEN)
            {
                array_check_word(result, result_len, in_content, in_len, config);
            }
            else if (config.check_method == BY_CHAR_LEN)
            {
                array_check_char(result, result_len, in_content, in_len, config);
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

        TEST_METHOD(AllChainEmpty)
        {
            do_test_at(7);
        }

        TEST_METHOD(AllChainCycle)
        {
            do_test_at(13);
        }

        TEST_METHOD(AllChainAddition)
        {
            do_test_at(29);
        }

        // -------------------------------------------------------------

        TEST_METHOD(ByWordNormal)
        {
            do_test_at(2);
        }

        TEST_METHOD(ByWordEmpty)
        {
            do_test_at(8);
        }

        TEST_METHOD(ByWordCycle)
        {
            do_test_at(14);
        }

        TEST_METHOD(ByWordSetHead)
        {
            do_test_at(17);
        }

        TEST_METHOD(ByWordSetTail)
        {
            do_test_at(18);
        }

        TEST_METHOD(ByWordSetBoth)
        {
            do_test_at(19);
        }

        // -------------------------------------------------------------

        TEST_METHOD(UniqueChainNormal)
        {
            do_test_at(3);
        }

        TEST_METHOD(UniqueChainEmpty)
        {
            do_test_at(9);
        }

        TEST_METHOD(UniqueCycle)
        {
            do_test_at(15);
        }

        TEST_METHOD(UniqueCycleAddition)
        {
            do_test_at(30);
        }

        // -------------------------------------------------------------

        TEST_METHOD(ByCharNormal)
        {
            do_test_at(4);
        }

        TEST_METHOD(ByCharEmpty)
        {
            do_test_at(10);
        }

        TEST_METHOD(ByCharCycle)
        {
            do_test_at(16);
        }

        // -------------------------------------------------------------

        TEST_METHOD(ByWordAllowCycleNormal)
        {
            do_test_at(5);
        }

        TEST_METHOD(ByWordAllowCycleEmpty)
        {
            do_test_at(11);
        }

        // -------------------------------------------------------------

        TEST_METHOD(ByCharAllowCycleNormal)
        {
            do_test_at(6);
        }

        TEST_METHOD(ByCharAllowCycleEmpty)
        {
            do_test_at(12);
        }
    };
}
