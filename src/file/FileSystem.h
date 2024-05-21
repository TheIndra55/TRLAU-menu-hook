#pragma once

#include "cdc/file/FileSystem.h"

// Gets the current file system
cdc::FileSystem* GetFS() noexcept;

// Gets the disk file system
cdc::FileSystem* GetDiskFS() noexcept;