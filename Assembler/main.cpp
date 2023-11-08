
#include "assembler.h"
int main()
{
	FileInfo file = FileInfoCtor("expr.txt");
	char* cs = {};
	Assembler Asm = {};
	AssemblerCtor(&Asm, cs, &file);
	AssemblyProgram(&Asm);
	CSOutputFile(&Asm, "C:\\Users\\Рузаль\\Desktop\\CPU\\SPU\\bytecode.txt");
}
