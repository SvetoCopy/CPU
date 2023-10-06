#include "test.h"
#include "string.h"

int FilesCmp(FileInfo* a, FileInfo* b) {
	if (a->n_lines != b->n_lines) return -1;

	for (int i = 0; i < a->n_lines; i++) {
		if (strcmp(a->text[i], b->text[i]) != 0) {
			return -1;
		}
	}
	return 0;
}

int Test() {
	FileInfo file = FileInfoCtor("test_files/test1.txt");
	FILE* out = {};

	fopen_s(&out, "out.txt", "w");
	ExecuteProgramm(&file, out);
	fclose(out);

	FileInfo res1 = FileInfoCtor("test_files/out1.txt");
	FileInfo res2 = FileInfoCtor("out.txt");

	if (FilesCmp(&res1, &res2) == 0) printf("TEST - SUCCESS");
	else printf("TEST - FAILED");

	return 0;
}