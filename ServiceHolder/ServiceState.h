#pragma once
#include <Windows.h>
#include <boost\thread\mutex.hpp>

namespace sch
{
	class ServiceState
		: public SERVICE_STATUS
		, private boost::noncopyable
	{
	public:
		ServiceState();
		void Initialize(SERVICE_STATUS_HANDLE hService, DWORD serviceType);
		void ReportStatus(DWORD stateUltimate, DWORD waitHint = 0);
		DWORD GetCurrentState() const;
	private:
		SERVICE_STATUS_HANDLE m_service;
		mutable boost::mutex m_mutex;
	};
}