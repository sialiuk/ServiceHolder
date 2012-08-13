#include "stdafx.h"
#include "HandleService.h"

namespace sch
{
	Handle::Handle(SC_HANDLE handle)
		: m_handle(handle)
	{
	}

	Handle::~Handle()
	{
		CloseServiceHandle(m_handle);
	}

	Handle::operator SC_HANDLE() const
	{
		return m_handle;
	}
	
}