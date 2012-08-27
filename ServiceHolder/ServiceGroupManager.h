#pragma once
#include "ServiceManager.h"
#include <string>
#include <vector>

namespace sch
{
	class ServiceGroupManager
		: private boost::noncopyable
	{
	public:
		void AddService(wchar_t* nameService, ServiceFunction serviceMain);
		void Install(DWORD startType, DWORD errorCtrl = SERVICE_ERROR_NORMAL);
		void StartServiceProcess();
	public:
		void Remove();
	private:
		std::vector<SERVICE_TABLE_ENTRY> servicesTable;
	};
}