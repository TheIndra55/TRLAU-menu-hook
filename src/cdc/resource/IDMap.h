#pragma once

#include <unordered_map>

#include "cdc/file/FileSystem.h"

namespace cdc
{
	class IDMap
	{
	private:
		std::unordered_map<unsigned int, const char*> m_map;

	public:
		IDMap();

		void Open(const char* fileName, cdc::FileSystem* fileSystem);
		const char* GetName(unsigned int id) const noexcept;
	};
}