
#include "assembler.h"
int main()
{
	FileInfo file = FileInfoCtor("expr.txt");
	CS cs = {};
	CSCtor(&cs, (file.n_lines) * (sizeof(int) + sizeof(double)));
	AssemblyProgram(&cs, &file);
	WriteByteCode(&cs, "bytecode.txt");
}
