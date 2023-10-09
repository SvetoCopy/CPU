#define _CRT_SECURE_NO_WARNINGS
#ifndef DISASM_DED
#define DISASM_DED

#include "..\resource\isa.h"
#include <string.h>
#include "..\resource\RuzalLib\super_io.h"
#include "..\resource\logger_.h"

const int COMMAND_SIZE = 10;


int DisassemblyCommand(char* str, char* res);
int DisassemblyProgramm(FileInfo* file, const char* res_file);


#endif // !DISASSEMBLER_DED
