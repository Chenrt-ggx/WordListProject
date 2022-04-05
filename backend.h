#ifndef BACKEND_H
#define BACKEND_H

#ifdef _DEBUG
#using "bin/core.dll"
#else
#using "bin/core.dll"
#endif

static inline int gen_chains_all(char** words, int len, char** result) {
	return core::WordChainCoreInterface::gen_chains_all((unsigned char**)words, len, (unsigned char**)result);
}

static inline int gen_chain_word(char** words, int len, char** result, char head, char tail, bool enable_loop) {
    return core::WordChainCoreInterface::gen_chain_word((unsigned char**)words, len, (unsigned char**)result, head, tail, enable_loop);
}

static inline int gen_chain_word_unique(char** words, int len, char** result) {
    return core::WordChainCoreInterface::gen_chain_word_unique((unsigned char**)words, len, (unsigned char**)result);
}

static inline int gen_chain_char(char** words, int len, char** result, char head, char tail, bool enable_loop) {
    return core::WordChainCoreInterface::gen_chain_char((unsigned char**)words, len, (unsigned char**)result, head, tail, enable_loop);
}

#endif
