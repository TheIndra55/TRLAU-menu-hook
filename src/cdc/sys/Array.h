#pragma once

namespace cdc
{
	template <typename T>
	class Array
	{
	public:
		unsigned int m_size;
		unsigned int m_cap;

		T* m_data;
	};
}