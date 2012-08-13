#pragma once
#include <Windows.h>

namespace sch
{
	class Handle
	{
	public:
		Handle(SC_HANDLE);
		~Handle();
		operator SC_HANDLE() const;
	private:
		Handle(const Handle&);
		Handle& operator=(const Handle&);
	private:
		SC_HANDLE m_handle;
	};
}