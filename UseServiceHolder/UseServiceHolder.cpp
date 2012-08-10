// UseServiceHolder.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <boost\thread\mutex.hpp>
#include <boost\thread\condition_variable.hpp>
#include "..\ServiceHolder\ServiceBase.h"
#include "..\ServiceHolder\ServiceManager.h"

TCHAR serviceName[] = L"UseServiceHolder";

class Myservice
	: public sch::ServiceBase
{
	enum
	{
		START,
		STOP,
		PAUSE
	};
	typedef boost::unique_lock<boost::mutex> Lock;
public:
	Myservice()
		: sch::ServiceBase(serviceName)
		, m_flag(0)
	{
		m_event = CreateEvent(NULL, FALSE, FALSE, NULL);
	}

	virtual void Run()
	{
		ReportStatus(SERVICE_START_PENDING, 2000);
		char DataBuffer[] = "This is some test data to write to the file.";
		DWORD numBytes = 0;
		HANDLE hFile = CreateFile(L"D:\\Test.txt",
									GENERIC_WRITE,
									FILE_SHARE_WRITE,
									NULL,
									OPEN_ALWAYS,
									FILE_ATTRIBUTE_NORMAL,
									NULL);
		ReportStatus(SERVICE_RUNNING);
		while(GetFlag() != STOP)
		{
			if(GetFlag() != PAUSE)
			{
				WriteFile(hFile,
						DataBuffer,
						sizeof(DataBuffer),
						&numBytes,
						NULL);
				FlushFileBuffers(hFile);
				::Sleep(2000);
			}
			else
			{
				ReportStatus(SERVICE_PAUSED);
				Lock lock(m_mutexNotify);
				m_cond.wait(lock);

				if(GetFlag() != STOP)
				{
					ReportStatus(SERVICE_RUNNING);
				}
			}
		}
		CloseHandle(hFile);
		ReportStatus(SERVICE_STOPPED);
	}

	virtual void OnStop()
	{
		if(GetFlag() == PAUSE)
		{
			SetFlag(STOP);
			Lock lock(m_mutexNotify);
			m_cond.notify_one();
		}
		SetFlag(STOP);
	}

	virtual void OnPause()
	{
		SetFlag(PAUSE);
	}

	virtual void OnStart()
	{
		SetFlag(START);
		Lock lock(m_mutexNotify);
		m_cond.notify_one();
	}

	void SetFlag(unsigned value)
	{
		Lock lock(m_mutex);
		m_flag = value;
	}

	unsigned GetFlag() const
	{
		Lock lock(m_mutex);
		return m_flag;
	}
private:
	HANDLE m_event;
	unsigned m_flag;
	mutable boost::mutex m_mutex;
	mutable boost::mutex m_mutexNotify;
	boost::condition_variable m_cond;
};

sch::ServiceManager servMan(serviceName);

int _tmain(int argc, _TCHAR* argv[])
{
	if( lstrcmpi( argv[1], TEXT("install")) == 0 )
    {
        servMan.Install(SERVICE_DEMAND_START);
        return 0;
    }
	
	if( lstrcmpi( argv[1], TEXT("remove")) == 0 )
    {
		servMan.Remove();
        return 0;
    }

	servMan.StartServiceProcess(sch::ServiceBase::ServiceFunction<Myservice>);

	return 0;
}

