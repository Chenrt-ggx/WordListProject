#pragma once

bool is_txt(const char* s);

void set_flag(bool& flag);

void parse_alpha(const char* s, char& param);

void read_file(const char* filename, char*& content, int& len);

int split_content(char* content, const int len, char* result[]);

void write_to_screen(char* result[], const int len);

void write_to_solution(char* result[], const int len);
