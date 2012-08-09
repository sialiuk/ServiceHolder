#include "stdafx.h"
#include "ServiceBase.h"

namespace sch
{
	ServiceBase::ServiceBase(const TCHAR* srviceName)
	{
		SERVICE_STATUS_HANDLE service = RegisterServiceCtrlHandlerEx(srviceName, HandlerCallback, this);
		m_stateService.Initialize(service);
	}

	ServiceBase::~ServiceBase()
	{
	}

	void ServiceBase::OnStop()
	{
	}

	void ServiceBase::OnStart()
	{
	}

	void ServiceBase::OnPause()
	{
	}

	void ServiceBase::ReportStatus(DWORD stateUltimate, DWORD waitHint)
	{
		m_stateService.ReportStatus(stateUltimate, waitHint);
	}

	DWORD WINAPI ServiceBase::HandlerCallback(DWORD control, DWORD eventType, PVOID eventDate, PVOID context)
	{

		ServiceBase* service = static_cast<ServiceBase*>(context);
		DWORD state = 0;
		switch(control)
		{
		case SERVICE_CONTROL_SHUTDOWN:
		case SERVICE_CONTROL_STOP:
			service->OnStop();
			state = SERVICE_STOP_PENDING;
			break;

		case SERVICE_CONTROL_PAUSE:
			service->OnPause();
			state = SERVICE_PAUSE_PENDING;
			break;

		case SERVICE_CONTROL_CONTINUE:
			service->OnStart();
			state = SERVICE_CONTINUE_PENDING;
			break;

		case SERVICE_CONTROL_INTERROGATE:
			break;
		
		case SERVICE_CONTROL_PARAMCHANGE:
		
		default:
			return ERROR_CALL_NOT_IMPLEMENTED;
		}
		service->ReportStatus(state);
		return NO_ERROR;
	}
}