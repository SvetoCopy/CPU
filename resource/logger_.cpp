#include "logger_.h"

int LoggerCtor(const char* name, Logger* logger) {
	logger->name = name;
	fopen_s(&(logger->file), logger->name, "w");
	logger->is_opened = true;
	return 0;
}

int LoggerDtor(Logger* logger) {
	fclose(logger->file);
	logger->file = nullptr;
	logger->name = nullptr;
	return 0;
}
