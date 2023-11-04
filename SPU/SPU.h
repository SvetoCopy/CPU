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
typedef double Value_t;
typedef char   VRam_t;


const int SIZEOF_COMMAND     = sizeof(int);
const int SIZEOF_VALUE       = sizeof(Value_t);
const int SIZEOF_ADDRESS_NUM = sizeof(int);
const int VISIBLE_RAM_PART   = 3;
const int VRam_LEN           = 625;
const int VRam_WIDTH         = 25;
const int REG_COUNT          = 4;
const int RAM_LEN            = 10000;

struct SPU {
	Stack   stack;

	#define DEF_REG(name, ...) Reg_t name;
	#include "..\resource\def_reg.h"
	#undef DEF_REG

	LogFile logfile;
	Stack   CallStack;
	RAM_t*  RAM;
	VRam_t* VRam;
};

int SPUCtor(SPU* spu);
int SPUDtor(SPU* spu);
int SPUDump(SPU* spu);
int SPUVerify(SPU* spu);

int DrawCircle(SPU* spu);
int VRamPrint(SPU* spu);
int ExecuteProgram(SPU* spu, CS* cs, FILE* out);

#endif