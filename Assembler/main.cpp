
#include "assembler.h"
int main()
{
	FileInfo file = FileInfoCtor("expr.txt");
	char* cs = {};
	Assembler Asm = {};
	AssemblerCtor(&Asm, cs, &file);
	AssemblyProgram(&Asm);
	AssemblerOutputFile(&Asm, "C:\\Users\\Рузаль\\Desktop\\CPU\\SPU\\bytecode.txt");
	return 0;
}
