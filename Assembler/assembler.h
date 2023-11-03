#define _CRT_SECURE_NO_WARNINGS
#ifndef ASSEMBLER_DED
#define ASSEMBLER_DED

// g++ -I main.cpp

#include "..\resource\isa.h"
#include <string.h>
#include <ctype.h>
#include "..\resource\RuzalLib\super_io.h"
#include "..\resource\RuzalLib\logger_.h"
#include <math.h>
#include "..\resource\utility.h"
#include "..\resource\CS.h"

const double POISON = -1;
const int COMMAND_SIZE = 100;
const int INITIAL_COUNT_LABELS = 100;
const int MAX_RAM_LEN = 1000;

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

int AssemblyProgram(Assembler* ASM);



#endif // !DISASSEMBLER_DED