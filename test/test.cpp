#include "pch.h"
#include "utils.h"
#include "CppUnitTest.h"
#include "../src/utils.cpp"
#include "../src/word_list.cpp"

#ifdef _DEBUG
    #pragma comment(lib, "../x64/Debug/core.lib")
#else
    #pragma comment(lib, "../x64/Release/core.lib")
#endif

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

        void core_test_at(const int index)
        {
            static char* result[20000];
            check_testcase(index, "coretest");
            string base = "../../cases/coretest/testcase" + to_string(index);
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

        void cli_test_at(const int index, bool has_ring = false)
        {
            static char* result[20000];
            static char* ring[] = { "Ab", "bc", "cD", "DA" };
            static char* chain[] = { "Ab", "bc", "cD", "DE" };
            filename = nullptr;
            param_h = param_t = '\0';
            flag_n = flag_w = flag_m = flag_c = flag_h = flag_t = flag_r = false;
            check_testcase(index, "clitest");
            string base = "../../cases/clitest/testcase" + to_string(index);
            read_by_line(base + ".in", in_content, in_len);
            read_by_line(base + ".config", config_content, config_len);
            create_temp_file("testcase.txt", has_ring ? ring : chain, sizeof(has_ring ? ring : chain) / sizeof(char*));
            cli_parse_check(main(in_len, in_content), config_content, config_len);
            remove_temp_file("testcase.txt");
            free_content(in_content, in_len);
            free_content(config_content, config_len);
            delete in_content;
            delete config_content;
        }

    public:
        TEST_METHOD(AllChainNormal) { core_test_at(1); }
        TEST_METHOD(AllChainEmpty) { core_test_at(7); }
        TEST_METHOD(AllChainCycle) { core_test_at(13); }
        TEST_METHOD(AllChainAddition) { core_test_at(29); }
        TEST_METHOD(AllChainExceed) { core_test_at(30); }

        // -------------------------------------------------------------

        TEST_METHOD(ByWordNormal) { core_test_at(2); }
        TEST_METHOD(ByWordEmpty) { core_test_at(8); }
        TEST_METHOD(ByWordCycle) { core_test_at(14); }
        TEST_METHOD(ByWordSetHead) { core_test_at(17); }
        TEST_METHOD(ByWordSetTail) { core_test_at(18); }
        TEST_METHOD(ByWordSetBoth) { core_test_at(19); }

        // -------------------------------------------------------------

        TEST_METHOD(UniqueChainNormal) { core_test_at(3); }
        TEST_METHOD(UniqueChainEmpty) { core_test_at(9); }
        TEST_METHOD(UniqueChainCycle) { core_test_at(15); }
        TEST_METHOD(UniqueChainAddition) { core_test_at(31); }
        TEST_METHOD(TestPerformance) { core_test_at(32); }

        // -------------------------------------------------------------

        TEST_METHOD(ByCharNormal) { core_test_at(4); }
        TEST_METHOD(ByCharEmpty) { core_test_at(10); }
        TEST_METHOD(ByCharCycle) { core_test_at(16); }
        TEST_METHOD(ByCharSetHead) { core_test_at(20); }
        TEST_METHOD(ByCharSetTail) { core_test_at(21); }
        TEST_METHOD(ByCharSetBoth) { core_test_at(22); }

        // -------------------------------------------------------------

        TEST_METHOD(ByWordAllowRNormal) { core_test_at(5); }
        TEST_METHOD(ByWordAllowREmpty) { core_test_at(11); }
        TEST_METHOD(ByWordAllowRSetHead) { core_test_at(23); }
        TEST_METHOD(ByWordAllowRSetTail) { core_test_at(24); }
        TEST_METHOD(ByWordAllowRSetBoth) { core_test_at(25); }

        // -------------------------------------------------------------

        TEST_METHOD(ByCharAllowRNormal) { core_test_at(6); }
        TEST_METHOD(ByCharAllowREmpty) { core_test_at(12); }
        TEST_METHOD(ByCharAllowRSetHead) { core_test_at(26); }
        TEST_METHOD(ByCharAllowRSetTail) { core_test_at(27); }
        TEST_METHOD(ByCharAllowRSetBoth) { core_test_at(28); }

        // -------------------------------------------------------------

        TEST_METHOD(FlagNotExistA) { cli_test_at(1); }
        TEST_METHOD(FlagNotExistB) { cli_test_at(2); }
        TEST_METHOD(FlagNotExistC) { cli_test_at(3); }
        TEST_METHOD(FlagNotExistD) { cli_test_at(4); }
        TEST_METHOD(FlagNotExistE) { cli_test_at(5); }
        TEST_METHOD(FlagNotExistF) { cli_test_at(6); }
        
        // -------------------------------------------------------------

        TEST_METHOD(FilenameNotExistA) { cli_test_at(7); }
        TEST_METHOD(FilenameNotExistB) { cli_test_at(8); }
        TEST_METHOD(FilenameNotExistC) { cli_test_at(9); }
        TEST_METHOD(FilenameNotExistD) { cli_test_at(10); }
        TEST_METHOD(FilenameNotExistE) { cli_test_at(11); }
        TEST_METHOD(FilenameNotExistF) { cli_test_at(12); }

        // -------------------------------------------------------------

        TEST_METHOD(FilenameDuplicateA) { cli_test_at(13); }
        TEST_METHOD(FilenameDuplicateB) { cli_test_at(14); }
        TEST_METHOD(FilenameDuplicateC) { cli_test_at(15); }
        TEST_METHOD(FilenameDuplicateD) { cli_test_at(16); }
        TEST_METHOD(FilenameDuplicateE) { cli_test_at(17); }
        TEST_METHOD(FilenameDuplicateF) { cli_test_at(18); }

        // -------------------------------------------------------------

        TEST_METHOD(FlagUndefinedA) { cli_test_at(19); }
        TEST_METHOD(FlagUndefinedB) { cli_test_at(20); }
        TEST_METHOD(FlagUndefinedC) { cli_test_at(21); }
        TEST_METHOD(FlagUndefinedD) { cli_test_at(22); }
        TEST_METHOD(FlagUndefinedE) { cli_test_at(23); }
        TEST_METHOD(FlagUndefinedF) { cli_test_at(37); }
        TEST_METHOD(FlagUndefinedG) { cli_test_at(38); }
        TEST_METHOD(FlagUndefinedH) { cli_test_at(39); }

        // -------------------------------------------------------------

        TEST_METHOD(ParamFormatErrorA) { cli_test_at(24); }
        TEST_METHOD(ParamFormatErrorB) { cli_test_at(25); }
        TEST_METHOD(ParamFormatErrorC) { cli_test_at(26); }
        TEST_METHOD(ParamFormatErrorD) { cli_test_at(27); }
        TEST_METHOD(ParamFormatErrorE) { cli_test_at(28); }
        TEST_METHOD(ParamFormatErrorF) { cli_test_at(29); }
        TEST_METHOD(ParamFormatErrorG) { cli_test_at(30); }
        TEST_METHOD(ParamFormatErrorH) { cli_test_at(31); }
        TEST_METHOD(ParamFormatErrorI) { cli_test_at(32); }

        // -------------------------------------------------------------

        TEST_METHOD(ParamNotExistA) { cli_test_at(33); }
        TEST_METHOD(ParamNotExistB) { cli_test_at(34); }
        TEST_METHOD(ParamNotExistC) { cli_test_at(35); }
        TEST_METHOD(ParamNotExistD) { cli_test_at(36); }

        // -------------------------------------------------------------

        TEST_METHOD(FlagConflictA) { cli_test_at(40); }
        TEST_METHOD(FlagConflictB) { cli_test_at(41); }
        TEST_METHOD(FlagConflictC) { cli_test_at(42); }
        TEST_METHOD(FlagConflictD) { cli_test_at(43); }
        TEST_METHOD(FlagConflictE) { cli_test_at(44); }
        TEST_METHOD(FlagConflictF) { cli_test_at(45); }
        TEST_METHOD(FlagConflictG) { cli_test_at(46); }
        TEST_METHOD(FlagConflictH) { cli_test_at(47); }
        TEST_METHOD(FlagConflictI) { cli_test_at(48); }
        TEST_METHOD(FlagConflictJ) { cli_test_at(49); }

        // -------------------------------------------------------------

        TEST_METHOD(FlagConflictSameA) { cli_test_at(50); }
        TEST_METHOD(FlagConflictSameB) { cli_test_at(51); }
        TEST_METHOD(FlagConflictSameC) { cli_test_at(52); }
        TEST_METHOD(FlagConflictSameE) { cli_test_at(53); }
        TEST_METHOD(FlagConflictSameF) { cli_test_at(54); }

        // -------------------------------------------------------------

        TEST_METHOD(FileNotExist) { cli_test_at(55); }
        TEST_METHOD(FileUnableOpen) { cli_test_at(56); }

        // -------------------------------------------------------------

        TEST_METHOD(CorrectA) { cli_test_at(57); }
        TEST_METHOD(CorrectB) { cli_test_at(58); }
        TEST_METHOD(CorrectC) { cli_test_at(59); }
        TEST_METHOD(CorrectD) { cli_test_at(60); }
        TEST_METHOD(CorrectE) { cli_test_at(61); }
        TEST_METHOD(CorrectF) { cli_test_at(62); }
        TEST_METHOD(CorrectG) { cli_test_at(63); }
        TEST_METHOD(CorrectH) { cli_test_at(64); }
        TEST_METHOD(CorrectI) { cli_test_at(65); }
        TEST_METHOD(CorrectJ) { cli_test_at(66); }
        TEST_METHOD(CorrectK) { cli_test_at(67); }
        TEST_METHOD(CorrectL) { cli_test_at(68); }

        // -------------------------------------------------------------

        TEST_METHOD(RingA) { cli_test_at(69, true); }
        TEST_METHOD(RingB) { cli_test_at(70, true); }
        TEST_METHOD(RingC) { cli_test_at(71, true); }
        TEST_METHOD(RingD) { cli_test_at(72, true); }
        TEST_METHOD(RingE) { cli_test_at(73, true); }
        TEST_METHOD(RingF) { cli_test_at(74, true); }
    };
}
