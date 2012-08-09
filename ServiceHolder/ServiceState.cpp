#include "stdafx.h"
#include "ServiceState.h"

namespace sch
{
	ServiceState::ServiceState()
		: m_service(NULL)
	{
		ZeroMemory(this, sizeof(SERVICE_STATUS));
	}

	void ServiceState::Initialize(SERVICE_STATUS_HANDLE hService)
	{
		m_service = hService;
		dwServiceType = SERVICE_WIN32_OWN_PROCESS;
		dwCurrentState = SERVICE_START_PENDING;
		dwControlsAccepted = 0;
		dwWin32ExitCode = NO_ERROR;
		dwServiceSpecificExitCode = 0;
		dwCheckPoint = 0; 
		dwWaitHint = 2000;
	}

	void ServiceState::ReportStatus(DWORD stateUltimate, DWORD waitHint)
	{
		dwCurrentState = stateUltimate;
		dwWaitHint = waitHint;
		switch(dwCurrentState)
		{
		case SERVICE_START_PENDING:
		case SERVICE_PAUSE_PENDING:
			dwControlsAccepted = 0;
			++dwCheckPoint;
			break;
		case SERVICE_RUNNING:
		case SERVICE_PAUSED:
			dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE;
			dwCheckPoint = 0;
			break;
		}
		::SetServiceStatus(m_service, this);
	}
}