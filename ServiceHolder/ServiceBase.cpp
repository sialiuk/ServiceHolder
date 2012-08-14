#include "stdafx.h"
#include <boost\format.hpp>
#include "ServiceBase.h"

namespace sch
{
	ServiceBase::ServiceBase(const wchar_t* serviceName, DWORD serviceType)
	{
		SERVICE_STATUS_HANDLE service = RegisterServiceCtrlHandlerEx(serviceName, HandlerCallback, this);
		if(service == NULL)
		{
			throw std::runtime_error((boost::format("Register Service Handler is failed, error: %1%") 
													% GetLastError()).str());
		}
		m_stateService.Initialize(service, serviceType);
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

	DWORD ServiceBase::GetCurrentState() const
	{
		return m_stateService.GetCurrentState();
	}

	DWORD WINAPI ServiceBase::HandlerCallback(DWORD control, DWORD eventType, PVOID eventDate, PVOID context)
	{
		ServiceBase* service = static_cast<ServiceBase*>(context);
		switch(control)
		{
		case SERVICE_CONTROL_SHUTDOWN:
		case SERVICE_CONTROL_STOP:
			service->ReportStatus(SERVICE_STOP_PENDING, 2000);
			service->OnStop();
			break;

		case SERVICE_CONTROL_PAUSE:
			service->ReportStatus(SERVICE_PAUSE_PENDING, 2000);
			service->OnPause();
			break;

		case SERVICE_CONTROL_CONTINUE:
			service->ReportStatus(SERVICE_CONTINUE_PENDING, 2000);
			service->OnStart();
			break;

		case SERVICE_CONTROL_INTERROGATE:
			service->ReportStatus(service->GetCurrentState());
			break;
		
		case SERVICE_CONTROL_PARAMCHANGE:
			return ERROR_CALL_NOT_IMPLEMENTED;

		default:
			return ERROR_CALL_NOT_IMPLEMENTED;
		}
		return NO_ERROR;
	}
}