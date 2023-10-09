#define _CRT_SECURE_NO_WARNINGS
#ifndef ASSEMBLER_DED
#define ASSEMBLER_DED

// g++ -I main.cpp

#include "../resource/iso.h"
#include <string.h>
#include <C:\Users\Рузаль\Desktop\onegin\onegin\onegin\super_io.h>
#include "C:\Users\Рузаль\Desktop\CPU\resource\logger_.h"
#include <math.h>

const int COMMAND_SIZE = 5;


int AssemblyCommand(char* str, char* res);
int AssemblyProgramm(FileInfo* file, const char* res_file);


#endif // !DISASSEMBLER_DED