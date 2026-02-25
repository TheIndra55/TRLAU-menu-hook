#pragma once

namespace cdc
{
	// Hopefully S here stands for silly, because whoever made this data structure was on crack
	template <typename T>
	class SArray
	{
	public:
		T* m_data;

		unsigned int size()
		{
			if (!m_data) return 0;
			return ((unsigned int*)m_data)[-1];
		}
	};
}