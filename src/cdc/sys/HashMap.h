#pragma once

#include "Hash.h"

namespace cdc
{
	template <typename K, typename V>
	class HashMap : public HashTable<K, V>
	{
	};
}