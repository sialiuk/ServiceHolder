#include "stdafx.h"
#include <cassert>
#include "Service.h"

namespace sch
{
	Service::Handle::Handle(SC_HANDLE handle)
		: m_handle(handle)
	{
	}

	Service::Handle::~Handle()
	{
		BOOL f = CloseServiceHandle(m_handle);
	}

	Service::Handle::operator SC_HANDLE() const
	{
		return m_handle;
	}

	Service::Service(LPWSTR serviceName, LPWSTR machineName)
		: m_serviceName(serviceName)
		, m_machineName(machineName)
	{
	}

	void Service::Install(DWORD startType, DWORD serviceType, DWORD errorCtrl)
	{
		Handle scManager = OpenSCManager(m_machineName, NULL, SC_MANAGER_CREATE_SERVICE);

		wchar_t modulePatchName[MAX_PATH];
		GetModuleFileName(NULL, modulePatchName, sizeof(modulePatchName)); 

		Handle hService = CreateService(scManager, m_serviceName, m_serviceName, 0, serviceType,
													startType, errorCtrl, modulePatchName, NULL, NULL, NULL, NULL, NULL);
	}

	void Service::StartServiceProcess(ServiceFunction serviceMain)
	{
		SERVICE_TABLE_ENTRY serviceTable[] = 
		{
			{m_serviceName, serviceMain},
			{NULL, NULL}
		};
		assert(StartServiceCtrlDispatcher(serviceTable));
	}

	void Service::Remove()
	{
		Handle scManager = OpenSCManager(m_machineName, NULL, SC_MANAGER_CREATE_SERVICE);
		Handle hService = OpenService(scManager, m_serviceName, DELETE);
		DeleteService(hService);
	}
}