#ifndef SPU_DED
#define SPU_DED
#include "..\resource\RuzalLib\super_io.h"
#include "..\resource\RuzalLib\stack.h"
#include "..\resource\isa.h"
#include "..\resource\RuzalLib\logger_.h"
#include "string.h"
#include "..\resource\utility.h"
#include "..\resource\CS.h"
#include <math.h>

typedef double Reg_t;
typedef double RAM_t;
typedef char GD_t;

const int GD_LEN = 625;
const int GD_WIDTH = 25;
const int REG_COUNT = 4;
const int RAM_LEN = 10000;

struct SPU {
	Stack   stack;
	Reg_t   rax;
	Reg_t   rbx;
	Reg_t   rcx;
	Reg_t   rdx;
	Reg_t   r8;
	Reg_t   r9;
	Reg_t   r10;
	LogFile logfile;
	size_t  labels_count;
	Stack   CallStack;
	RAM_t* RAM;
	GD_t* GD;
};

int SPUCtor(SPU* spu);
int SPUDtor(SPU* spu);
int SPUDump(SPU* spu);
int SPUVerify(SPU* spu);

int DrawCircle(SPU* spu);
int GDPrint(SPU* spu);
int ExecuteProgramm(SPU* spu, CS* cs, FILE* out);

#endif