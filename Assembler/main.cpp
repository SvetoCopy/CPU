
#include "assembler.h"
int main()
{
	FileInfo file = FileInfoCtor("expr.txt");
	CS cs = {};
	CSCtor(&cs, (file.n_lines) * (sizeof(int) + sizeof(double)));
	Assembler Asm = {};
	AssemblerCtor(&Asm, &cs, &file);
	AssemblyProgram(&Asm);
	CSOutputFile(Asm.cs, "..\\SPU\\bytecode.txt");
}
