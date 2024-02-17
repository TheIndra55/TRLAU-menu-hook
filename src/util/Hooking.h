#pragma once

#include <initializer_list>

// Helpers for hooking and calling functions
class Hooking
{
public:
	// Calls a function
	template<typename... Args>
	static inline void Call(unsigned int address, Args... args) noexcept
	{
		reinterpret_cast<void(*)(Args...)>(address)(args...);
	}

	// Calls a function with a return value
	template<typename T, typename... Args>
	static inline T CallReturn(unsigned int address, Args... args) noexcept
	{
		return reinterpret_cast<T(*)(Args...)>(address)(args...);
	}

	// Calls a class function
	template<typename... Args>
	static inline void ThisCall(unsigned int address, Args... args) noexcept
	{
		reinterpret_cast<void(__thiscall*)(Args...)>(address)(args...);
	}

	// Calls a class function with a return value
	template<typename T, typename... Args>
	static inline T ThisCallReturn(unsigned int address, Args... args) noexcept
	{
		return reinterpret_cast<T(__thiscall*)(Args...)>(address)(args...);
	}

	// Sets a memory region to no-op
	static void Nop(void* address, unsigned int length) noexcept;

	// Patches data at a memory region
	static void Patch(void* address, std::initializer_list<unsigned char> data) noexcept;

	// Patches data at a memory region
	static void Patch(void* address, unsigned int data) noexcept;
};

#if TR7
#define GET_ADDRESS(tr7, trae, tr8) tr7
#elif TRAE
#define GET_ADDRESS(tr7, trae, tr8) trae
#elif TR8
#define GET_ADDRESS(tr7, trae, tr8) tr8
#endif