#ifndef CONTROL_HOLDING_H
#define CONTROL_HOLDING_H

#include <assert.h>

namespace WinApiFramework
{
	template <class T> struct ControlPointer
	{
		T* m_pControlPointer;
		int m_userHolders, m_parentHolders;

		ControlPointer(T* controlPointer, int userHolders, int parentHolders)
			: m_pControlPointer(controlPointer)
			, m_userHolders(userHolders)
			, m_parentHolders(parentHolders)
		{}
		~ControlPointer()
		{
			assert(m_pControlPointer == 0);
		}
	};

	template <class T> struct ControlHolder
	{
	private:
		ControlPointer* m_pControlPointer;

	public:
		ControlHolder()
		{

		}

	public:
		T* operator->()
		{
			return static_cast<T*>(m_pControlPointer->m_pControlPointer);
		}
	};
}

#endif //!CONTROL_HOLDING_H