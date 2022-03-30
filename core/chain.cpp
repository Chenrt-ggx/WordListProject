#include "pch.h"
#include "chain.h"

#include <string>
#include <cassert>

using namespace std;

char* Chain::at(const int index) const
{
    return word_vector[index]->get_word();
}

int Chain::get_word_len() const
{
    return (int)word_vector.size();
}

int Chain::get_char_len() const
{
    return char_len;
}

void Chain::push_back(Node* word)
{
    word_vector.push_back(word);
    char_len += word->get_len();
}

void Chain::pop_back()
{
    char_len -= word_vector.back()->get_len();
    word_vector.pop_back();
}

char* Chain::to_string() const
{
    string str;
    for (auto& i : word_vector)
    {
        str += i->get_word();
        str += " ";
    }
    char* result = (char*)malloc(str.length() + 1);
    assert(result != nullptr);
    strcpy_s(result, str.length() + 1, str.c_str());
    return result;
}
