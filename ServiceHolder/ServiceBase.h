#pragma once
#include "ServiceState.h"

namespace sch
{
	class ServiceBase
	{
	public:
		explicit ServiceBase(const wchar_t* srviceName);
		virtual ~ServiceBase();
		virtual void OnStop();
		virtual void OnStart();
		virtual void OnPause();
		virtual void Run() = 0;
		void ReportStatus(DWORD stateUltimate, DWORD waitHint = 0);
		DWORD GetCurrentState() const;
		template<class T>
			static VOID WINAPI ServiceFunction(DWORD, PTSTR*)
			{
				T service;
				service.Run();
			}
	private:
		static DWORD WINAPI HandlerCallback(DWORD control, DWORD eventType, PVOID eventDate, PVOID context);
	private:
		ServiceState m_stateService;
	};
}