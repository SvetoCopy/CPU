#ifndef SPU_DED
#define SPU_DED
#include "..\resource\RuzalLib\super_io.h"
#include "..\resource\RuzalLib\stack.h"
#include "..\resource\isa.h"
#include "..\resource\RuzalLib\logger_.h"
#include "string.h"
#include "..\resource\utility.h"
#include "..\resource\CS.h"

typedef double Reg_t;

const int REG_COUNT = 4;


struct SPU {
	Stack   stack;
	Reg_t   rax;
	Reg_t   rbx;
	Reg_t   rcx;
	Reg_t   rdx;
	LogFile logfile;
};

int SPUCtor(SPU* spu);
int SPUDtor(SPU* spu);
int SPUDump(SPU* spu);
int SPUVerify(SPU* spu);

int ReadCommand(CS* cs, size_t* ip, Opcode* opcode);
int ReadRegArg(CS* cs, size_t* ip, int* reg_num);
int ReadImmArg(CS* cs, size_t* ip, double* value);
int ReadArg(CS* cs, size_t* ip, Opcode opcode, double* value, int* reg_num);

int ExecuteProgramm(CS* cs, FILE* out);

int CSInsert(CS* cs, FileInfo* file);
#endif