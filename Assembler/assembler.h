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

const double POISON = -1;
const int COMMAND_SIZE = 10;
const int INITIAL_COUNT_LABELS = 10;

struct Label {
    char* name;
    double address;
};

struct Assembler {
    CS* cs;
    FileInfo* file;
    Label* labels;
    int labels_count;
};

int AssemblerCtor(Assembler* ASM, CS* cs, FileInfo* file);
int AssemblerDtor(Assembler* ASM);
int AssemblerDump(Assembler* ASM);
int AssemblerVerify(Assembler* ASM);

int LabelCtor(Label* label, char* name, double address);
int LabelDtor(Label* label);
int LabelDump(Label* label);
int LabelVerify(Label* label);

int CSInsert(int arg_type, CS* cs, double* value, int* reg_num, int* command);
int CSInsertDoubleCode(CS* cs, double* code);
int CSInsertIntCode(CS* cs, int* code);

int DetermineLabel(Assembler* ASM, char* str);
int DetermineReg(char* str);   
int CreateLabel(Assembler* ASM, char* name, double address);

int AssemblyLabels(Assembler* ASM);
int AssemblyStr(Assembler* ASM, char* str, int* type_code, int* reg_num, double* value, size_t passage);
int AssemblyImm(char* str, int* type_code, double* value);
int AssemblyArg(Assembler* ASM, char* str, int* type_code, int arg_count, int* reg_num, double* value, size_t passage);
int AssemblyCommand(char* str, Assembler* ASM, size_t passage);
int AssemblyProgram(Assembler* ASM);

int WriteByteCode(CS* cs, const char* filename);

#endif // !DISASSEMBLER_DED