#pragma once

// Helpers for hooking and calling functions
class Hooking
{
public:
	// Calls a function
	template<typename... Args>
	static inline void Call(unsigned int address, Args... args)
	{
		reinterpret_cast<void(*)(Args...)>(address)(args...);
	}

	// Calls a function with a return value
	template<typename T, typename... Args>
	static inline T CallReturn(unsigned int address, Args... args)
	{
		return reinterpret_cast<T(*)(Args...)>(address)(args...);
	}

	// Calls a class function
	template<typename... Args>
	static inline void ThisCall(unsigned int address, Args... args)
	{
		reinterpret_cast<void(__thiscall*)(Args...)>(address)(args...);
	}

	// Calls a class function with a return value
	template<typename T, typename... Args>
	static inline T ThisCallReturn(unsigned int address, Args... args)
	{
		return reinterpret_cast<T(__thiscall*)(Args...)>(address)(args...);
	}
};

#if TR7
#define GET_ADDRESS(tr7, trae, tr8) trl
#elif TRAE
#define GET_ADDRESS(tr7, trae, tr8) trae
#elif TR8
#define GET_ADDRESS(tr7, trae, tr8) tr8
#endif