#ifndef SPU_DED
#define SPU_DED
#include "..\resource\RuzalLib\super_io.h"
#include "..\resource\RuzalLib\stack.h"
#include "..\resource\isa.h"
#include "..\resource\RuzalLib\logger_.h"
#include "string.h"
#include "..\resource\utility.h"

typedef double Reg_t;

enum Errors_SPU {
	CS_RANGE_ERROR = 0,
	CS_NULLPTR = 1
};

struct SPU {
	Stack   stack;
	Reg_t   rax;
	Reg_t   rbx;
	Reg_t   rcx;
	Reg_t   rdx;
	int     ip;
	size_t  CS_size;
	char*   CS;
	LogFile logfile;
};

int SPUCtor(SPU* spu, FileInfo* file);
int SPUDtor(SPU* spu);
int SPUDump(SPU* spu);
int SPUVerify(SPU* spu);

int ReadCommand(char* str, int* command, double* value, int* reg_num);
int ExecuteProgramm(FileInfo* file, FILE* out);

int CSInsert(char* CS, char* text);
#endif