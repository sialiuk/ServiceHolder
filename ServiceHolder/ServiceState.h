#pragma once

#include <Windows.h>

namespace sch
{
	class ServiceState
		: public SERVICE_STATUS
	{
	public:
		ServiceState();
		void Initialize(SERVICE_STATUS_HANDLE hService);
		void ReportStatus(DWORD stateUltimate, DWORD waitHint = 0);
		DWORD GetCurrentState() const;
	private:
		SERVICE_STATUS_HANDLE m_service;
	};

}