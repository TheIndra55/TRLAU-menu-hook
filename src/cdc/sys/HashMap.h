#pragma once

#include <utility>

#include "Array.h"

namespace cdc
{
	template <typename K, typename V>
	class HashTable
	{
	public:
		class Node
		{
		public:
			Node* m_next;
			std::pair<K, V> m_data;
		};

		cdc::Array<Node*> m_buckets;
		unsigned int m_size;
	};
}