#include "assembler.h"

int main(int argc, const char* argv[])
{
	if (argc < 3) {
		printf("Usage: %s filename.asm output.bin\n", argv[0]);
		return -1;
	}
		
	FileInfo file = FileInfoCtor(argv[1]);
	char* cs = {};
	Assembler Asm = {};

	AssemblerCtor(&Asm, cs, &file);
	AssemblyProgram(&Asm);
	AssemblerOutputFile(&Asm, argv[2]);

	return 0;
}
