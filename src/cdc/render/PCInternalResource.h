#pragma once

namespace cdc
{
	class PCInternalResource
	{
	private:
		PCInternalResource* m_pPrev;
		PCInternalResource* m_pNext;

	protected:
		void OnConstruct();

	public:
		PCInternalResource();

		virtual ~PCInternalResource();
		virtual bool OnCreateDevice() = 0;
		virtual void OnDestroyDevice() = 0;

#if TR8
		virtual bool OnPostResetDevice();
		virtual void OnPreResetDevice();
		virtual void OnSettingsChanged();
#endif
	};
}