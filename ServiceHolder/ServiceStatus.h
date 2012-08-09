#pragma once
#include <Windows.h>

namespace sch
{
	class ServiceStatus
		: public SERVICE_STATUS
	{
	public:
		ServiceStatus();
		void Initialize(LPCWSTR serviceName, LPHANDLER_FUNCTION_EX handlerFunction, PVOID pvContext);
		void AcceptControls(DWORD dwFlags);
		bool SetUltimateStatus(DWORD dwUltimateState, DWORD dwWaitHint = 0);
		bool ReportUltimateStatus();
		bool ReportStatus();
		DWORD CurrentState()const;
	private:
		SERVICE_STATUS_HANDLE m_ssh;
	};
}