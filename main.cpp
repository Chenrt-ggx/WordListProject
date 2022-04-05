#include <iostream>
#include "backend.h"

char space[20000][1024];
char* result[20000];
char* words[] = {
    "woo",
    "oom",
    "moon",
    "noox"
};

int main()
{
    for (int i = 0; i < 20000; i++) {
        result[i] = space[i];
    }
    int result_len = gen_chains_all(words, sizeof(words) / sizeof(char*), result);
    std::cout << result_len << std::endl;
    for (int i = 0; i < result_len; i++) {
        std::cout << result[i] << std::endl;
    }
    return 0;
}
