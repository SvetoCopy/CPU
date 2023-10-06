#ifndef SPU_DED
#define SPU_DED
#include <C:\Users\Рузаль\Desktop\onegin\onegin\onegin\super_io.h>
#include <C:\Users\Рузаль\Desktop\stackk\stackk\stack.h>
#include "C:\Users\Рузаль\Desktop\CPU\resource\iso.h"
#include <C:\Users\Рузаль\Desktop\CPU\resource\logger_.h>

struct Command {
	int value;
	int factor;
	int type;
};

struct CPU {
	Stack stack;
};


// struct Loader
// {
//    char* [];
// }

int CPUDtor(CPU* cpu);

int ReadCommand(char* str, Command* command);
int ExecuteProgramm(FileInfo* file, FILE* out);
#endif