#ifndef SPU_DED
#define SPU_DED
#include <C:\Users\Рузаль\Desktop\onegin\onegin\onegin\super_io.h>
#include <C:\Users\Рузаль\Desktop\stackk\stackk\stack.h>
#include "C:\Users\Рузаль\Desktop\CPU\resource\iso.h"
#include <C:\Users\Рузаль\Desktop\CPU\resource\logger_.h>


const int FACTOR = 3;

struct Command {
	int value;
	int type;
};

// 32 bit - ARM RISC-V
// 
// 8 bit
// 64 = 2^6
// 9 bit - 16 bit
// struct Opcode
// {
//		int code: 6;
//		int arg_type : 2;
// 
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
};


// struct Loader
// {
//    char* [];
// }
int SPUCtor(SPU* spu);
int SPUDtor(SPU* spu);
int SPUDump(SPU* spu);
int SPUVerify(SPU* spu);

int ReadCommand(char* str, Command* command);
int ExecuteProgramm(FileInfo* file, FILE* out);
#endif