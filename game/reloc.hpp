#pragma once
#include <Windows.h>

struct PEHANDLE
{
	IMAGE_NT_HEADERS* lpHeader;
	IMAGE_SECTION_HEADER* lpSectionTable;
	int firstReloc;
	IMAGE_DOS_HEADER* lpImage;
};

int __cdecl MakePeHandle(_IMAGE_DOS_HEADER* peData, PEHANDLE* pe);
