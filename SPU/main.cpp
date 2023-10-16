#include "SPU.h"
//#include "test.h"
int main()
{
	FileInfo file = FileInfoCtor("bytecode.txt");
	FILE* out = {};
	fopen_s(&out, "out.txt", "wb");

	CS cs = {};
	CSCtor(&cs, file.buff_size);
	CSInsert(&cs, &file);
	ExecuteProgramm(&cs, out);

	CSDtor(&cs);
	fclose(out);
	FileInfoDtor(&file);

}
