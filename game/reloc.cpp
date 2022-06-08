#include "reloc.hpp"

int __cdecl MakePeHandle(_IMAGE_DOS_HEADER* peData, PEHANDLE* pe)
{
	pe->lpImage = peData;

	if (peData->e_magic != IMAGE_DOS_SIGNATURE)
	{
		return RELOC_ERROR_NON_EXECUTABLE;
	}

	IMAGE_NT_HEADERS* header = (IMAGE_NT_HEADERS*)((int)peData + peData->e_lfanew);
	pe->lpHeader = header;
	pe->firstReloc = 1;

	if (header->Signature != IMAGE_NT_SIGNATURE)
	{
		if (header->Signature != 0x4551)
		{
			return RELOC_ERROR_NON_EXECUTABLE;
		}

		pe->firstReloc = 0;
	}

	if (header->FileHeader.Machine != IMAGE_FILE_MACHINE_I386)
	{
		return RELOC_ERROR_NON_EXECUTABLE;
	}

	auto flags = header->FileHeader.Characteristics;

	if ((flags & IMAGE_FILE_EXECUTABLE_IMAGE) == 0 || (flags & IMAGE_FILE_DLL) == 0)
	{
		return RELOC_ERROR_NON_EXECUTABLE;
	}

	pe->lpSectionTable = IMAGE_FIRST_SECTION(header);

	if (header->FileHeader.NumberOfSections <= 0)
	{
		// not sure why this would be success, but as in orginal game code
		return RELOC_SUCCESS;
	}

	for (int i = 0; i < header->FileHeader.NumberOfSections; i++)
	{
		auto section = pe->lpSectionTable[i];

		if (section.Misc.VirtualSize > section.SizeOfRawData)
		{
			// actually valid in Windows PE loader, but rejected in the orginal game code
			return RELOC_ERROR_INVALID_SECTION;
		}

#if GENERAL_FIXES
		// check if section is executable
		if (pe->firstReloc && (section.Characteristics & IMAGE_SCN_CNT_CODE) != 0)
		{
			// mark the section memory as PAGE_EXECUTE so DEP isn't triggered
			DWORD lpflOldProtect;
			VirtualProtect((void*)((int)peData + section.PointerToRawData), section.SizeOfRawData, PAGE_EXECUTE_READWRITE, &lpflOldProtect);
		}
#endif

		pe->lpSectionTable[i].Misc.VirtualSize = section.SizeOfRawData;
	}

	return RELOC_SUCCESS;
}
