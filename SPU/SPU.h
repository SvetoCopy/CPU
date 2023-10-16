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

int ReadCommand(char** CS_ptr, size_t* ip, Opcode* opcode);
int ExecuteProgramm(CS* cs, FILE* out);

int CSInsert(char** CS, FileInfo* file);
#endif