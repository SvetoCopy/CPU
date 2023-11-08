#include "SPU.h"
//#include "test.h"
int main()
{
	FileInfo file = FileInfoCtor("bytecode.txt");
	FILE* out = {};
	fopen_s(&out, "out.txt", "wb");

	CodeSeg  cs  = {};
	SPU spu = {};
	CodeSegCtor(&cs, file.buff_size);
	SPUCtor(&spu);
	CodeSegInsertFile(&cs, &file);
	//DrawCircle(&spu);
	//VRamPrint(&spu);
	ExecuteProgram(&spu, &cs, out);

	CodeSegDtor(&cs);
	fclose(out);
	FileInfoDtor(&file);

}
