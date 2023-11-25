#ifndef ALPHABET_DED
#define ALPHABET_DED
// instruction set architecture
#define DEF_CMD(name, code, ...) name = code,

enum Commands {
	#include "def_cmd.h"
	#undef DEF_CMD
};

#endif