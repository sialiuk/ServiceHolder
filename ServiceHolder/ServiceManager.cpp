#include "stdafx.h"
#include <cassert>
#include "ServiceManager.h"

namespace sch
{
	ServiceManager::Handle::Handle(SC_HANDLE handle)
		: m_handle(handle)
	{
	}

	ServiceManager::Handle::~Handle()
	{
		CloseServiceHandle(m_handle);
	}

	ServiceManager::Handle::operator SC_HANDLE() const
	{
		return m_handle;
	}

	ServiceManager::ServiceManager(LPWSTR serviceName, LPWSTR machineName)
		: m_serviceName(serviceName)
		, m_machineName(machineName)
	{
	}

	void ServiceManager::Install(DWORD startType, DWORD errorCtrl)
	{
		Handle scManager = OpenSCManager(m_machineName, NULL, SC_MANAGER_CREATE_SERVICE);

		wchar_t modulePatchName[MAX_PATH];
		GetModuleFileName(NULL, modulePatchName, sizeof(modulePatchName)); 

		Handle hService = CreateService(scManager, m_serviceName, m_serviceName, 0, SERVICE_WIN32_OWN_PROCESS,
													startType, errorCtrl, modulePatchName, NULL, NULL, NULL, NULL, NULL);
	}

	void ServiceManager::StartServiceProcess(ServiceFunction serviceMain)
	{
		SERVICE_TABLE_ENTRY serviceTable[] = 
		{
			{m_serviceName, serviceMain},
			{NULL, NULL}
		};
		BOOL b = StartServiceCtrlDispatcher(serviceTable);
		DWORD error = GetLastError();
	}

	void ServiceManager::Remove()
	{
		Handle scManager = OpenSCManager(m_machineName, NULL, SC_MANAGER_CONNECT);
		Handle hService = OpenService(scManager, m_serviceName, DELETE);
		DeleteService(hService);
	}
}