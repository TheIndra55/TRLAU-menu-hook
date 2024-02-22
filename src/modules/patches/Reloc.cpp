#include "Reloc.h"

int MakePeHandle(IMAGE_DOS_HEADER* peData, PEHANDLE* pe)
{
	pe->lpImage = peData;

	if (peData->e_magic != IMAGE_DOS_SIGNATURE)
	{
		return RELOC_NON_EXECUTABLE;
	}

	auto header = (IMAGE_NT_HEADERS*)((char*)peData + peData->e_lfanew);
	pe->lpHeader = header;
	pe->firstReloc = 1;

	// Check for the PE signature or the relocated signature
	if (header->Signature != IMAGE_NT_SIGNATURE)
	{
		if (header->Signature != 0x4551)
		{
			return RELOC_NON_EXECUTABLE;
		}

		pe->firstReloc = 0;
	}

	if (header->FileHeader.Machine != IMAGE_FILE_MACHINE_I386)
	{
		return RELOC_NON_EXECUTABLE;
	}

	auto flags = header->FileHeader.Characteristics;

	if ((flags & IMAGE_FILE_EXECUTABLE_IMAGE) == 0 || (flags & IMAGE_FILE_DLL) == 0)
	{
		return RELOC_NON_EXECUTABLE;
	}

	// Read the sections
	pe->lpSectionTable = IMAGE_FIRST_SECTION(header);

	if (header->FileHeader.NumberOfSections <= 0)
	{
		return RELOC_SUCCESS;
	}

	for (int i = 0; i < header->FileHeader.NumberOfSections; i++)
	{
		auto section = pe->lpSectionTable[i];

		if (section.Misc.VirtualSize > section.SizeOfRawData)
		{
			// Since there is no virtual memory allocated for the sections the raw data size
			// cannot be smaller than the virtual size
			return RELOC_INVALID_SECTION;
		}

		// Check if the section is executable
		if (pe->firstReloc && (section.Characteristics & IMAGE_SCN_CNT_CODE) != 0)
		{
			// Change the protection of the memory region to executable to allow code to execute
			// even with the Data Execution Prevention (DEP) enabled in Windows.
			DWORD oldProtect;
			VirtualProtect((void*)((char*)peData + section.PointerToRawData), section.SizeOfRawData, PAGE_EXECUTE_READWRITE, &oldProtect);
		}

		// Set the virtual size to the raw size since there's no virtual memory allocated
		pe->lpSectionTable[i].Misc.VirtualSize = section.SizeOfRawData;
	}

	return RELOC_SUCCESS;
}