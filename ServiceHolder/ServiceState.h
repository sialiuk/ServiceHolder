#pragma once
#include <Windows.h>
#include <boost\thread\mutex.hpp>

namespace sch
{
	class ServiceState
		: public SERVICE_STATUS
	{
	public:
		ServiceState();
		void Initialize(SERVICE_STATUS_HANDLE hService, DWORD serviceType);
		void ReportStatus(DWORD stateUltimate, DWORD waitHint = 0);
		DWORD GetCurrentState() const;
	private:
		ServiceState(const ServiceState&);
		ServiceState& operator=(const ServiceState&);
	private:
		SERVICE_STATUS_HANDLE m_service;
		mutable boost::mutex m_mutex;
	};

}