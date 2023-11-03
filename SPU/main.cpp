#include "SPU.h"
//#include "test.h"
int main()
{
	FileInfo file = FileInfoCtor("bytecode.txt");
	FILE* out = {};
	fopen_s(&out, "out.txt", "wb");

	CS  cs  = {};
	SPU spu = {};
	CSCtor(&cs, file.buff_size);
	SPUCtor(&spu);
	CSInsertFile(&cs, &file);
	DrawCircle(&spu);
	GDPrint(&spu);
	ExecuteProgramm(&spu, &cs, out);

	CSDtor(&cs);
	fclose(out);
	FileInfoDtor(&file);

}
