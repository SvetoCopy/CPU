#define _CRT_SECURE_NO_WARNINGS
#ifndef ASSEMBLER_DED
#define ASSEMBLER_DED

#include "..\resource\isa.h"
#include <string.h>
#include <ctype.h>
#include "..\resource\RuzalLib\super_io.h"
#include "..\resource\RuzalLib\logger_.h"
#include <math.h>
#include "..\resource\utility.h"

const double POISON               = -1;
const int    COMMAND_SIZE         = 100;
const int    INITIAL_COUNT_LABELS = 100;
const int    MAX_RAM_LEN          = 1000;

enum ParseStatus {
    PARSE_ERROR = -1,
    CMD_UNFOUND = 0,
    FOUND       = 1
};

struct Label {
    char*  name;
    double address;
};

struct Assembler {
    LogFile   logfile;
    FileInfo* file;

    char*     code_seg;
    size_t    code_seg_size;

    Label*    labels;
    int       labels_count;
};

int AssemblerCtor(Assembler* ASM, char* CodeSeg, FileInfo* file);
int AssemblerDtor(Assembler* ASM);

int AssemblerOutputFile(Assembler* ASM, const char* filename);
int AssemblerDump(Assembler* ASM);
int AssemblerVerify(Assembler* ASM);

int AssemblyProgram(Assembler* ASM);
#endif // !DISASSEMBLER_DED