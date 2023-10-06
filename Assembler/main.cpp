
#include "assembler.h"
#define DEF_CMD(name, code, ...) printf("%s\n", #name);
int main()
{
	#include "C:\Users\Рузаль\Desktop\CPU\resource\def_cmd.h"

	FileInfo file = FileInfoCtor("expr.txt");
	AssemblyProgramm(&file, "res.txt");
}
