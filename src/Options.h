#pragma once

#include <Windows.h>
#include <vector>

#include "modules/Module.h"
#include "Hook.h"

template <typename T>
class Option : public BaseOption
{
private:
	T m_value;

public:
	Option(const char* name, T defaultValue) : BaseOption(name), m_value(defaultValue)
	{
		Hook::GetInstance().GetModule<Options>()->AddOption(this);
	}

	// Loads the value from the registry
	void LoadValue()
	{
		HKEY hKey;
		if (RegOpenKeyA(HKEY_CURRENT_USER, "SOFTWARE\\TRAE-Menu-Hook", &hKey) == ERROR_SUCCESS)
		{
			auto size = sizeof(T);
			RegQueryValueExA(hKey, m_name, NULL, NULL, (LPBYTE)&m_value, (LPDWORD)&size);

			RegCloseKey(hKey);
		}
	}

	// Saves the current value to the registry
	void SaveValue()
	{
		HKEY hKey;
		if (RegCreateKeyA(HKEY_CURRENT_USER, "SOFTWARE\\TRAE-Menu-Hook", &hKey) == ERROR_SUCCESS)
		{
			RegSetValueExA(hKey, m_name, NULL, REG_BINARY, (LPBYTE)&m_value, sizeof(T));
			RegCloseKey(hKey);
		}
	}

	// Gets the type hash
	size_t GetType()
	{
		return typeid(T).hash_code();
	}

	// Gets a pointer to the value
	void* GetValuePtr()
	{
		return &m_value;
	}

	// Gets the value
	T GetValue()
	{
		return m_value;
	}
};

class BaseOption
{
protected:
	const char* m_name;

public:
	BaseOption(const char* name);

	// Gets the name
	const char* GetName()
	{
		return m_name;
	}

	// Methods to be implemented by Option<T>
	virtual void LoadValue() = 0;
	virtual void SaveValue() = 0;
	virtual size_t GetType() = 0;
	virtual void* GetValuePtr() = 0;
};

class Options : public Module
{
private:
	std::vector<BaseOption*> m_options;
	bool m_show = false;

	void DrawOptions();
	void DrawOption(const char* name, const char* description, float min = 0.f, float max = 0.f);
	void DrawOption(BaseOption* option, const char* description, float min, float max);

public:
	Options();

	void OnDraw();
	void Show();
	void AddOption(BaseOption* option);
};