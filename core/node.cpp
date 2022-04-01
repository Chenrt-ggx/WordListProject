#include "pch.h"
#include "node.h"

#include <cstring>

int Node::get_len() const
{
    return len;
}

int Node::get_end() const
{
    return end;
}

char* Node::get_word() const
{
    return word;
}

int Node::get_status() const
{
    return status;
}

void Node::set_status(const int status)
{
    this->status = status;
}

bool Node::operator==(const Node& rhs) const
{
    return strcmp(this->word, rhs.word) == 0;
}

bool Node::operator<(const Node& rhs) const
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
