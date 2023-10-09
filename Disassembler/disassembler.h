#define _CRT_SECURE_NO_WARNINGS
#ifndef DISASM_DED
#define DISASM_DED

#include "C:\Users\Рузаль\Desktop\CPU\resource\iso.h"
#include <string.h>
#include <C:\Users\Рузаль\Desktop\onegin\onegin\onegin\super_io.h>
#include "C:\Users\Рузаль\Desktop\CPU\resource\logger_.h"

const int COMMAND_SIZE = 10;


int DisassemblyCommand(char* str, char* res);
int DisassemblyProgramm(FileInfo* file, const char* res_file);


#endif // !DISASSEMBLER_DED
