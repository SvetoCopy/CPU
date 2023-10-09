#include "SPU.h"
#include "test.h"
int main()
{
	FileInfo file = FileInfoCtor("expr.txt");
	FILE* out = {};

	fopen_s(&out, "out.txt", "w");
	ExecuteProgramm(&file, out);
	fclose(out);
	FileInfoDtor(&file);

}
