#pragma once

#include <filesystem>

#include "Module.h"
#include "file/MultiFileSystem.h"

class ModLoader : public Module
{
private:
	MultiFileSystem m_fileSystem;

	void MountArchive(std::filesystem::path& name) noexcept;
	void MountDirectory(std::filesystem::path& name) noexcept;

public:
	ModLoader();
	void MountMods();

	MultiFileSystem* GetFileSystem() noexcept;
};