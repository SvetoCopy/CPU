#define _CRT_SECURE_NO_WARNINGS
#ifndef ASSEMBLER_DED
#define ASSEMBLER_DED

// g++ -I main.cpp

#include "..\resource\isa.h"
#include <string.h>
#include "..\resource\RuzalLib\super_io.h"
#include "..\resource\RuzalLib\logger_.h"
#include <math.h>
#include "..\resource\utility.h"

const int COMMAND_SIZE = 10;
const int REG_SIZE = 3;

struct ASM {
	FileInfo* file_inp;
	FileInfo* file_res;
	LogFile   logger;
};
// ???
int ASMCtor();
int ASMDtor();
int ASMVerify();
int ASMDump();

int DetermineReg(char* str);
int AssemblyCommand(char* str, FILE* res);
int AssemblyProgramm(FileInfo* file, const char* res_file);


#endif // !DISASSEMBLER_DED