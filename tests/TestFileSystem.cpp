#include <catch_amalgamated.hpp>

#include "cdc/file/FileSystem.h"
#include "file/MultiFileSystem.h"

class DummyFileSystem : public cdc::FileSystem
{
public:
	cdc::FileRequest* RequestRead(cdc::FileReceiver* receiver, const char* fileName, unsigned int startOffset) { return nullptr; }
	cdc::File* OpenFile(const char* fileName) { return nullptr; }
	bool FileExists(const char* fileName) { return false; }
	unsigned int GetFileSize(const char* fileName) { return 0; }
	void SetSpecialisationMask(unsigned int specMask) { }
	unsigned int GetSpecialisationMask() { return 0; }
	Status GetStatus() { return IDLE; }
	void Update() { }
	void Synchronize() { }
};

TEST_CASE("multifilesystem test")
{
	auto multiFileSystem = new MultiFileSystem();

	SECTION("can add and remove systems")
	{
		auto fileSystem1 = new DummyFileSystem();
		auto fileSystem2 = new DummyFileSystem();

		multiFileSystem->Add(fileSystem1);
		multiFileSystem->Add(fileSystem2);

		REQUIRE(multiFileSystem->GetSystems().size() == 2);

		multiFileSystem->Remove(fileSystem1);

		REQUIRE(multiFileSystem->GetSystems().size() == 1);
	}

	SECTION("can check if file exists")
	{
		multiFileSystem->Add(new DummyFileSystem());
		multiFileSystem->Add(new DummyFileSystem());

		REQUIRE(multiFileSystem->FileExists("lara.drm") == false);
	}
}