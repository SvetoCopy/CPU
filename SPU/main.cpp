#include "SPU.h"

int main(int argc, const char* argv[])
{
	if (argc < 3) {
		printf("Usage: %s filename.bin output.txt\n", argv[0]);
		return -1;
	}
	FileInfo file = FileInfoCtor(argv[1]);
	FILE* out = {};
	fopen_s(&out, argv[2], "wb");
	CodeSeg  cs  = {};
	SPU spu = {};
	CodeSegCtor(&cs, file.buff_size);
	SPUCtor(&spu);
	CodeSegInsertFile(&cs, &file);
	ExecuteProgram(&spu, &cs, out);

	CodeSegDtor(&cs);
	fclose(out);
	FileInfoDtor(&file);

}
