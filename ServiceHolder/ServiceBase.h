#pragma once
#include "ServiceState.h"

namespace sch
{
	class ServiceBase
	{
	public:
		ServiceBase(const TCHAR* srviceName);
		virtual ~ServiceBase();
		virtual void OnStop();
		virtual void OnStart();
		virtual void OnPause();
		virtual void Run() = 0;
		void ReportStatus(DWORD stateUltimate, DWORD waitHint = 0);
	private:
		static DWORD WINAPI HandlerCallback(DWORD control, DWORD eventType, PVOID eventDate, PVOID context);
	public:
		template<class T>
			static VOID WINAPI ServiceFunction(DWORD, PTSTR*)
			{
				T service;
				service.Run();
			}
	private:
		ServiceState m_stateService;
	};
}