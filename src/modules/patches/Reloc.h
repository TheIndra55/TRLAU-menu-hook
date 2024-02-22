#pragma once

#include <Windows.h>

struct PEHANDLE
{
	IMAGE_NT_HEADERS* lpHeader;
	IMAGE_SECTION_HEADER* lpSectionTable;
	int firstReloc;
	IMAGE_DOS_HEADER* lpImage;
};

// This enum does not exist in the PDB, therefore the names are guessed
enum RelocCode
{
	RELOC_INVALID_SECTION = -1,
	RELOC_SUCCESS = 0,
	RELOC_NON_EXECUTABLE = 3,
};

int MakePeHandle(IMAGE_DOS_HEADER* peData, PEHANDLE* pe);