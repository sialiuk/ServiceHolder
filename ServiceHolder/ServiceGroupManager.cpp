#include "stdafx.h"
#include <vector>
#include "ServiceGroupManager.h"
#include "HandleService.h"
#include <boost\format.hpp>

namespace sch
{
	void ServiceGroupManager::AddService(wchar_t* nameService, ServiceFunction serviceMain)
	{
		SERVICE_TABLE_ENTRY table = {nameService, serviceMain};
		servicesTable.push_back(table);
	}

	void ServiceGroupManager::Install(DWORD startType, DWORD errorCtrl)
	{
		wchar_t modulePatchName[MAX_PATH];
		DWORD result = GetModuleFileName(NULL, modulePatchName, sizeof(modulePatchName));
		if(result == 0)
		{
			throw std::runtime_error((boost::format("Get module file name is failed, error: %1%") 
													% GetLastError()).str());
		}

		Handle scManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
		if(scManager == NULL)
		{
			throw std::runtime_error((boost::format("Open SC Manager is failed, error: %1%") 
														% GetLastError()).str());
		}

		for(size_t i = 0; i != servicesTable.size(); ++i)
		{
			Handle hService = CreateService(scManager, servicesTable[i].lpServiceName, servicesTable[i].lpServiceName, 0, SERVICE_WIN32_SHARE_PROCESS,
														startType, errorCtrl, modulePatchName, NULL, NULL, NULL, NULL, NULL);
			if(hService == NULL)
			{
				throw std::runtime_error((boost::format("Create service is failed, error: %1%") 
														% GetLastError()).str());
			}
		}
	}

	void ServiceGroupManager::StartServiceProcess()
	{
		SERVICE_TABLE_ENTRY table = {NULL, NULL};
		servicesTable.push_back(table);
		if(!StartServiceCtrlDispatcher(&servicesTable[0]))
		{
			throw std::runtime_error((boost::format("Start service is failed, error: %1%") 
													% GetLastError()).str());
		}
	}

	void ServiceGroupManager::Remove()
	{
		Handle scManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
		if(scManager == NULL)
		{
			throw std::runtime_error((boost::format("Open SC Manager is failed, error: %1%") 
													% GetLastError()).str());
		}

		for(size_t i = 0; i != servicesTable.size(); ++i)
		{
			Handle hService = OpenService(scManager, servicesTable[i].lpServiceName, DELETE);

			if(hService == NULL)
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
}