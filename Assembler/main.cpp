
#include "assembler.h"
int main()
{
	FileInfo file = FileInfoCtor("expr.txt");
	AssemblyProgramm(&file, "res.txt");
}
