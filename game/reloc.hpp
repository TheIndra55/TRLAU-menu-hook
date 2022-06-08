#pragma once
#include <Windows.h>

struct PEHANDLE
{
	IMAGE_NT_HEADERS* lpHeader;
	IMAGE_SECTION_HEADER* lpSectionTable;
	int firstReloc;
	IMAGE_DOS_HEADER* lpImage;
};

// guessed enum, does not exist in pdb
enum RelocCode
{
	RELOC_ERROR_INVALID_SECTION = -1,
	RELOC_SUCCESS = 0,
	RELOC_ERROR_NON_EXECUTABLE = 3,
};

int __cdecl MakePeHandle(_IMAGE_DOS_HEADER* peData, PEHANDLE* pe);
