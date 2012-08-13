#pragma once
#include "ServiceManager.h"
#include <string>
#include <vector>

namespace sch
{
	class ServiceGroupManager
	{
	public:
		void AddService(wchar_t* nameService, ServiceFunction serviceMain);
		void Install(DWORD startType, DWORD errorCtrl = SERVICE_ERROR_NORMAL);
		void StartServiceProcess();
		void Remove();
	private:
		wchar_t* m_nameServiceGroup;
		std::vector<SERVICE_TABLE_ENTRY> servicesTable;
	};
}