#ifndef UTILITY_DED
#define UTILITY_DED
typedef double Reg_t;

const int REG_SIZE = 3;
const int ERROR = -1;

enum ArgType {
	UNDEFINED = -1,
	IMM = 0,
	REG = 1,
	RAM_IMM = 2,
	RAM_REG = 3,
	EMPTY = 4,
	LABEL = 5,
};

enum Registers {
	RAX = 0,
	RBX = 1,
	RCX = 2,
	RDX = 3,
};

struct Opcode
{
	unsigned int code : 6;
	unsigned int arg_type : 2;
};


#endif