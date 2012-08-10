#include "stdafx.h"
#include <stdexcept>
#include <boost\format.hpp>
#include "HandleService.h"
#include "ServiceManager.h"

namespace sch
{
	ServiceManager::ServiceManager(wchar_t* serviceName, ServiceFunction serviceMain)
		: m_serviceName(serviceName)
		, m_serviceMain(serviceMain)
	{
	}

	void ServiceManager::Install(DWORD startType, DWORD errorCtrl)
	{
		wchar_t modulePatchName[MAX_PATH];
		DWORD result = GetModuleFileName(NULL, modulePatchName, sizeof(modulePatchName));
		if(result == 0)
		{
			throw std::runtime_error((boost::format("Get module file name is failed, error: %1%") 
													% GetLastError()).str());
		}

		Handle scManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
		Handle hService = CreateService(scManager, m_serviceName, m_serviceName, 0, SERVICE_WIN32_OWN_PROCESS,
													startType, errorCtrl, modulePatchName, NULL, NULL, NULL, NULL, NULL);
		if(scManager == NULL || hService == NULL)
		{
			throw std::runtime_error((boost::format("Install service is failed, error: %1%") 
													% GetLastError()).str());
		}
	}

	void ServiceManager::StartServiceProcess()
	{
		SERVICE_TABLE_ENTRY serviceTable[] = 
		{
			{m_serviceName, m_serviceMain},
			{NULL, NULL}
		};

		if(!StartServiceCtrlDispatcher(serviceTable))
		{
			throw std::runtime_error((boost::format("Start service is failed, error: %1%") 
													% GetLastError()).str());
		}
	}

	void ServiceManager::Remove()
	{
		Handle scManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
		Handle hService = OpenService(scManager, m_serviceName, DELETE);

		if(scManager == NULL || hService == NULL)
		{
			throw std::runtime_error((boost::format("Open service is failed, error: %1%") 
													% GetLastError()).str());
		}

		if(!DeleteService(hService))
		{
			throw std::runtime_error((boost::format("Remove service is failed, error: %1%") 
													% GetLastError()).str());
		}
	}
}