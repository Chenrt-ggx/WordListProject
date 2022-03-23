#pragma once

constexpr int E_FLAG_NOT_DEFINED = 1;
constexpr int E_FLAG_CONFLICT = 1 << 1;
constexpr int E_FLAG_PARAM_FORMAT_ERROR = 1 << 2;
constexpr int E_FLAG_PARAM_NOT_EXISTS = 1 << 3;
constexpr int E_FLAG_NOT_EXISTS = 1 << 4;

constexpr int E_FILENAME_NOT_PROVIDED = 1 << 5;
constexpr int E_FILE_NOT_EXISTS = 1 << 6;
constexpr int E_FILE_UNABLE_TO_OPEN = 1 << 7;
constexpr int E_MEMORY_ALLOC_FAILED = 1 << 8;

constexpr int E_WORD_CYCLE_EXISTS = 1 << 9;


