#pragma once

#include "cdc/file/FileSystem.h"

// Gets the current file system
cdc::FileSystem* GetFS();

// Reads a file and returns the read buffer
char* FSHelper_ReadFile(const char* fileName, cdc::FileSystem* fileSystem);