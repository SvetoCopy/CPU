
#include "disassembler.h"
int main()
{
	FileInfo file = FileInfoCtor("expr.txt");
	DisassemblyProgramm(&file, "res.txt");
}
