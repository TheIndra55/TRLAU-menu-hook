#pragma once

#include "Hash.h"

namespace cdc
{
	template <typename K>
	class HashSet : public HashTable<K, K>
	{
	};
}