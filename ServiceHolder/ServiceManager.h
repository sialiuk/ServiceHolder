#pragma once
#include <boost\noncopyable.hpp>
#include <Windows.h>

namespace sch
{
	typedef VOID (WINAPI *ServiceFunction)(DWORD, PTSTR*);

	class ServiceManager
		: private boost::noncopyable
	{
	public:
		explicit ServiceManager(wchar_t* serviceName, ServiceFunction serviceMain);
		void Install(DWORD startType, DWORD errorCtrl = SERVICE_ERROR_NORMAL);
		void StartServiceProcess();
		void Remove();
	private:
		wchar_t* m_serviceName;
		ServiceFunction m_serviceMain;
	};
}