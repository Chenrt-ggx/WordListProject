from ctypes import *
from typing import List

dll = WinDLL(r".\\core.dll")


class WordCycleException(Exception):
    pass


def gen_chain_word(words: List[bytes], head: int, tail: int, enable_loop: bool):
    # int gen_chain_word(char* words[], int len, char* result[], char head, char tail, bool enable_loop)
    c_words = (c_char_p * len(words))()
    for i, word in enumerate(words):
        c_words[i] = word
    c_len = c_int(len(words))
    c_result = (c_char_p * 20000)()
    c_head = c_char(head)
    c_tail = c_char(tail)
    c_enable_loop = c_bool(enable_loop)
    count = dll.gen_chain_word(c_words, c_len, c_result, c_head, c_tail, c_enable_loop)
    if count < 0:
        raise WordCycleException
    result = [None] * count
    for i in range(count):
        result[i] = c_result[i]
    return result


def gen_chains_all(words: List[bytes]):
    # int gen_chains_all(char* words[], int len, char* result[])
    c_words = (c_char_p * len(words))()
    for i, word in enumerate(words):
        c_words[i] = word
    c_len = c_int(len(words))
    c_result = (c_char_p * 20000)()
    count = dll.gen_chains_all(c_words, c_len, c_result)
    if count < 0:
        raise WordCycleException
    result = [None] * count
    if count <= 20000:
        for i in range(count):
            result[i] = c_result[i]
    return result


def gen_chain_word_unique(words: List[bytes]):
    # int gen_chain_word_unique(char* words[], int len, char* result[])
    c_words = (c_char_p * len(words))()
    for i, word in enumerate(words):
        c_words[i] = word
    c_len = c_int(len(words))
    c_result = (c_char_p * 20000)()
    count = dll.gen_chain_word_unique(c_words, c_len, c_result)
    if count < 0:
        raise WordCycleException
    result = [None] * count
    for i in range(count):
        result[i] = c_result[i]
    return result


def gen_chain_char(words: List[bytes], head: int, tail: int, enable_loop: bool):
    # int gen_chain_char(char* words[], int len, char* result[], char head, char tail, bool enable_loop)
    c_words = (c_char_p * len(words))()
    for i, word in enumerate(words):
        c_words[i] = word
    c_len = c_int(len(words))
    c_result = (c_char_p * 20000)()
    c_head = c_char(head)
    c_tail = c_char(tail)
    c_enable_loop = c_bool(enable_loop)
    count = dll.gen_chain_char(c_words, c_len, c_result, c_head, c_tail, c_enable_loop)
    if count < 0:
        raise WordCycleException
    result = [None] * count
    for i in range(count):
        result[i] = c_result[i]
    return result


if __name__ == '__main__':
    # test()
    r = gen_chain_word([b'hello', b'foo', b'oxx'], 0, 0, False)
    print(r)
