#include <Windows.h>

#include "Hooking.h"

void Hooking::Nop(void* address, unsigned int length) noexcept
{
	DWORD oldProtect;
	VirtualProtect(address, length, PAGE_EXECUTE_READWRITE, &oldProtect);

	memset(address, 0x90, length);

	VirtualProtect(address, length, oldProtect, &oldProtect);
}

void Hooking::Patch(void* address, std::initializer_list<unsigned char> data) noexcept
{
	DWORD oldProtect;
	VirtualProtect(address, data.size(), PAGE_EXECUTE_READWRITE, &oldProtect);

	memcpy(address, data.begin(), data.size());

	VirtualProtect(address, data.size(), oldProtect, &oldProtect);
}

void Hooking::Patch(void* address, unsigned int data) noexcept
{
	DWORD oldProtect;
	VirtualProtect(address, sizeof(data), PAGE_EXECUTE_READWRITE, &oldProtect);

	memcpy(address, &data, sizeof(data));

	VirtualProtect(address, sizeof(data), oldProtect, &oldProtect);
}