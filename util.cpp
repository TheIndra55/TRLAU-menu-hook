#include <Windows.h>
#include <psapi.h>
#include "Hooking.hpp"

// taken from sunbeam906
uint8_t __declspec(noinline)* GetAddress(uint8_t* ptr, uint8_t offset, uint8_t instr_size)
{
    return (ptr + *(int32_t*)(ptr + offset) + instr_size);
}

bool __declspec(noinline) bDataCompare(const BYTE* pData, const BYTE* bMask, const char* szMask)
{
    for (; *szMask; ++szMask, ++pData, ++bMask)
    {
        if (*szMask == 'x' && *pData != *bMask)
            return FALSE;
    }

    return (*szMask) == NULL;
}

uint8_t __declspec(noinline)* FindPattern(BYTE* bMask, char* szMask)
{
    MODULEINFO moduleInfo = { 0 };
    GetModuleInformation(GetCurrentProcess(), GetModuleHandle(NULL), &moduleInfo, sizeof(MODULEINFO));

    uint8_t* dwBaseAddress = (uint8_t*)moduleInfo.lpBaseOfDll + 0x1000; // start from first section
    int dwModuleSize = (int)moduleInfo.SizeOfImage - 0x1000; // scan size - 0x1000, due to the above

    for (int i = 0; i < dwModuleSize; i++)
    {
        if (bDataCompare((BYTE*)(dwBaseAddress + i), bMask, szMask))
            return (uint8_t*)(dwBaseAddress + i);
    }
    return 0;
}
