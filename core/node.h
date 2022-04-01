#pragma once

class Node
{
private:
    int len;
    int end;
    int status;
    char* word;

public:
    Node(int len, char* word) : len(len), word(word), status(0), end(word[len - 1]) {}

    int get_len() const;

    int get_end() const;

    char* get_word() const;

    int get_status() const;
    
    void set_status(const int status);

    bool operator==(const Node& rhs) const;
};
