
#include "assembler.h"
int main()
{
	FileInfo file = FileInfoCtor("expr.txt");
	ASM as = {};
	ASMCtor(&as, &file);
	WriteByteCode(&as, "bytecode.txt");
}
