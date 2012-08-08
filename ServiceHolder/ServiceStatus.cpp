#include "stdafx.h"
#include "ServiceStatus.h"

namespace sch
{
	ServiceStatus::ServiceStatus()
		: m_ssh(NULL)
	{
		 ZeroMemory(this, sizeof(SERVICE_STATUS));
	}

	void ServiceStatus::Initialize(LPCWSTR serviceName, LPHANDLER_FUNCTION_EX handlerFunction, PVOID pvContext)
	{
		m_ssh = RegisterServiceCtrlHandlerEx(serviceName, handlerFunction, pvContext); //exception
		dwServiceType = SERVICE_WIN32_OWN_PROCESS;
		dwCurrentState = SERVICE_START_PENDING;
		dwControlsAccepted = 0; 
		dwWin32ExitCode = NO_ERROR;
		dwServiceSpecificExitCode = 0;
		dwCheckPoint = 0; 
		dwWaitHint = 2000;
	}

	void ServiceStatus::AcceptControls(DWORD dwFlags)
	{
		dwControlsAccepted |= dwFlags;
	}

	bool ServiceStatus::SetUltimateStatus(DWORD dwUltimateState, DWORD dwWaitHint)
	{
		DWORD dwPendingState = 0;
		switch(dwUltimateState)
		{
		case SERVICE_RUNNING:
			dwPendingState = (dwCurrentState == SERVICE_PAUSED) 
				? SERVICE_CONTINUE_PENDING : SERVICE_START_PENDING;
			break;
		case SERVICE_PAUSED:
			dwPendingState = SERVICE_PAUSE_PENDING;
			break;
		case SERVICE_STOPPED:
			dwPendingState = SERVICE_STOP_PENDING;
			break;
		}

		dwCheckPoint = 1;
		this->dwWaitHint = dwWaitHint;
		bool flag = false;
		if(dwPendingState)
		{
			dwCurrentState = dwPendingState;
			flag = (dwWaitHint != 0) ? ReportStatus() : ReportUltimateStatus();
		}
		return flag;
	}

	bool ServiceStatus::ReportUltimateStatus()
	{
		DWORD dwUltimateState = 0;
		switch(dwCurrentState)
		{
		case SERVICE_CONTINUE_PENDING:
		case SERVICE_START_PENDING:
			dwUltimateState = SERVICE_RUNNING;
			break;
		case SERVICE_PAUSE_PENDING:
			dwUltimateState = SERVICE_PAUSED;
			break;
		case SERVICE_STOP_PENDING:
			dwUltimateState = SERVICE_STOPPED;
			break;
		}
		dwCheckPoint = dwWaitHint = 0;
		bool flag = false;

		if (dwUltimateState != 0) 
		{
			dwCurrentState = dwUltimateState;
			flag = ReportStatus();
		}
		return flag;
	}

	bool ServiceStatus::ReportStatus()
	{
		if(::SetServiceStatus(m_ssh, this))
		{
			return true;
		}
		return false;
	}
}