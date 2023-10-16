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
#include "..\resource\CS.h"

const int COMMAND_SIZE = 10;
                                      
int CSInsert(int arg_type, CS* cs, double* value, int* reg_num, int* command);
int CSInsertDoubleCode(CS* cs, double* code);
int CSInsertIntCode(CS* cs, int* code);

int DetermineReg(char* str);   

int AssemblyReg(char* str, int* type_code, int* reg_num);
int AssemblyImm(char* str, int* type_code, double* value);
int AssemblyArg(char* str, int* type_code, int arg_count, int* reg_num, double* value);
int AssemblyCommand(char* str, CS* cs);
int AssemblyProgram(CS* cs, FileInfo* file);

int WriteByteCode(CS* cs, const char* filename);

#endif // !DISASSEMBLER_DED