#ifndef SPU_DED
#define SPU_DED
#include "..\resource\RuzalLib\super_io.h"
#include "..\resource\RuzalLib\stack.h"
#include "..\resource\isa.h"
#include "..\resource\RuzalLib\logger_.h"
#include "string.h"
#include "..\resource\utility.h"

typedef double Reg_t;

const int FACTOR = 3;




// 32 bit - ARM RISC-V
// 
// 8 bit
// 64 = 2^6
// 9 bit - 16 bit


// char* = load_binary()
// RISC 
// CISC - x86
// 1 byte - 15 byte

// execute_code()
// {
//     Opcode opcode = *(Opcode*)code;
//	   opcode->code = ;
//     

struct SPU {
	Stack stack;
	Reg_t rax;
	Reg_t rbx;
	Reg_t rcx;
	Reg_t rdx;
	int ip;
	size_t CS_size;
	char* CS;
};


// struct Loader
// {
//    char* [];
// }
int SPUCtor(SPU* spu, FileInfo* file);
int SPUDtor(SPU* spu);
int SPUDump(SPU* spu);
int SPUVerify(SPU* spu);

int ReadCommand(char* str, int* command, double* value, int* reg_num);
int ExecuteProgramm(FileInfo* file, FILE* out);
#endif