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
const int ERROR = -1;
enum CommandArg{
	IMM = 0,
	REG = 1,
	EMPTY = 2,
};

struct ASM {
	char*   CS;
	int     CS_size;
	size_t  ip;                                                                             
	LogFile logger;                                                                    
};                                                                                
// ???                                                                               
int ASMCtor(ASM* asm_ptr, FileInfo* file);                                                 
int ASMDtor(ASM* asm_ptr);                                                            
int ASMVerify(ASM* asm_ptr);                                                         
int ASMDump(ASM* asm_ptr);                                                       
                                                                                   
int CSInsert(int arg_type,char** CS, double* value, int* reg_num, int* type_code);                             
int DetermineReg(char* str);                                                                     
int AssemblyCommand(char* str, double* value, int* reg_num, int* type_code);                     
int WriteByteCode(ASM* asm_ptr, const char* filename);

#endif // !DISASSEMBLER_DED