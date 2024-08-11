#include <sstream>

#include "IDMap.h"
#include "file/FileSystem.h"

cdc::IDMap::IDMap() : m_map()
{
}

void cdc::IDMap::Open(const char* fileName, cdc::FileSystem* fileSystem)
{
	auto buffer = FSHelper_ReadFile(fileName, fileSystem);

	if (buffer)
	{
		std::istringstream stream(buffer);

		unsigned int unk, count;

		// Read the header
		stream >> unk;
		stream.ignore(1);
		stream >> count;

		std::string line;
		std::getline(stream, line);

		// Read all lines
		for (unsigned int i = 0; i < count; i++)
		{
			int id;

			stream >> id;
			stream.ignore(1);

			std::getline(stream, line);

			// Strip any extra values
			line = line.substr(0, line.find(","));

			// Allocate new string
			auto name = new char[line.size() + 1];
			strcpy(name, line.c_str());

			m_map.insert({ id, name });
		}

		// Clean up
		delete buffer;
	}
}
const char* cdc::IDMap::GetName(unsigned int id) const noexcept
{
	auto name = m_map.find(id);

	if (name == m_map.end())
	{
		return "";
	}

	return name->second;
}