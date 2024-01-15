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
		reinterpret_cast<T(*)(Args...)>(address)(args...);
	}
};