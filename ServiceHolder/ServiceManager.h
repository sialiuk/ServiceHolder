#pragma once
#include <string>
#include <Windows.h>

namespace sch
{
	class ServiceManager
	{
		class Handle
		{
		public:
			Handle(SC_HANDLE);
			~Handle();
			operator SC_HANDLE() const;
		private:
			Handle(const Handle&);
			Handle& operator=(const Handle&);
		private:
			SC_HANDLE m_handle;
		};

		typedef VOID (WINAPI *ServiceFunction)(DWORD, PTSTR*);

	public:
		explicit ServiceManager(LPWSTR serviceName, LPWSTR machineName = NULL);
		void Install(DWORD startType, DWORD errorCtrl = SERVICE_ERROR_NORMAL);
		void StartServiceProcess(ServiceFunction serviceMain);
		void Remove();
	private:
		ServiceManager(const ServiceManager&);
		ServiceManager& operator=(const ServiceManager&);
	private:
		LPWSTR m_serviceName;
		LPWSTR m_machineName;
	};
}