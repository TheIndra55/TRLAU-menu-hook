#include <Hooking.Patterns.h>
#include <MinHook.h>

#include "ModLoader.h"
#include "Hook.h"
#include "Log.h"
#include "util/Hooking.h"

#include "cdc/file/MultiFileSystem.h"
#include "cdc/file/ArchiveFileSystem.h"

#include "file/FileSystem.h"
#include "file/HookFileSystem.h"
#include "file/MultiFileSystem.h"
#include "file/ModFileSystem.h"

static ModLoader* loader = nullptr;

static cdc::FileSystem* CreateHookFileSystem()
{
	auto fileSystem = new HookFileSystem(*(cdc::FileSystem**)GET_ADDRESS(0x10E58C0, 0x838890, 0x9CE27C));

	return fileSystem;
}

// Hooks
static bool(*s_InitPatchArchive)(const char*);
static void(*s_InitArchive)();

static bool InitPatchArchive(const char* name)
{
	// Let the game add the patch archive to the file system
	auto ret = s_InitPatchArchive(name);

	// Create our hook file system
	auto fileSystem = CreateHookFileSystem();

	// Add the hook file system in front of all other file systems
	auto multiFileSystem = (cdc::MultiFileSystem*)GetFS();
	multiFileSystem->Add(fileSystem, false, true);

	return ret;
}

static void InitArchive()
{
	// Let the game initialize the archive file system
	s_InitArchive();

	// Create our hook file system and multi file system
	auto fileSystem = CreateHookFileSystem();
	auto multiFileSystem = loader->GetFileSystem();

	loader->MountMods();

	multiFileSystem->Add(fileSystem);
	multiFileSystem->Add(GetFS());

	// Overwrite the original file system with ours
	*(cdc::FileSystem**)GET_ADDRESS(0x10E58BC, 0x83888C, 0x9CE278) = multiFileSystem;
}

// Initialize the mod loader and insert all hooks
ModLoader::ModLoader() : m_fileSystem()
{
	loader = this;

#ifndef TR8
	MH_CreateHook((void*)GET_ADDRESS(0x45C670, 0x45F5B0, 0x473840), InitArchive, (void**)&s_InitArchive);
#else
	MH_CreateHook((void*)0x478930, InitPatchArchive, (void**)&s_InitPatchArchive);
#endif

	MH_EnableHook(MH_ALL_HOOKS);
}

void ModLoader::MountMods()
{
	auto log = Hook::GetInstance().GetModule<Log>();

	for (auto& entry : std::filesystem::directory_iterator("mods"))
	{
		auto name = entry.path().filename();

		// Mount files with bigfile extension as archive
		if (entry.is_regular_file() && name.extension() == ".000")
		{
			log->WriteLine("Mounting mod archive %s", name.string().c_str());

			MountArchive(name);
		}

		// Mount folders as new mod file system
		if (entry.is_directory())
		{
			log->WriteLine("Mounting mod directory %s", name.string().c_str());

			MountDirectory(name);
		}
	}
}

void ModLoader::MountArchive(std::filesystem::path& name) noexcept
{
	auto path = "mods" / name;

	// Open the archive
	auto archive = new cdc::ArchiveFileSystem(*(cdc::FileSystem**)0x838890);
	archive->Open(path.string().c_str());

	// Mount the archive
	m_fileSystem.Add((cdc::FileSystem*)archive);
}

void ModLoader::MountDirectory(std::filesystem::path& name) noexcept
{
	auto path = "mods" / name;
	auto fileSystem = new ModFileSystem(name.string().c_str());

	// Mount the file system
	m_fileSystem.Add(fileSystem);
}

MultiFileSystem* ModLoader::GetFileSystem() noexcept
{
	return &m_fileSystem;
}