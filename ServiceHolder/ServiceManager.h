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
		explicit ServiceManager(wchar_t* serviceName, wchar_t* machineName = NULL);
		void Install(DWORD startType, DWORD errorCtrl = SERVICE_ERROR_NORMAL);
		void StartServiceProcess(ServiceFunction serviceMain);
		void Remove();
	private:
		ServiceManager(const ServiceManager&);
		ServiceManager& operator=(const ServiceManager&);
	private:
		wchar_t* m_serviceName;
		wchar_t* m_machineName;
	};
}