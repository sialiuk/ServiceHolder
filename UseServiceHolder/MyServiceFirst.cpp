#include "stdafx.h"
#include "MyServiceFirst.h"

namespace service
{
	MyserviceFirst::MyserviceFirst()
		: sch::ServiceBase(L"UseServiceHolder", SERVICE_WIN32_SHARE_PROCESS)
		, m_flag(START)
	{
	}

	MyserviceFirst::MyserviceFirst(TCHAR* name)
		: sch::ServiceBase(name, SERVICE_WIN32_SHARE_PROCESS)
		, m_flag(START)
	{
	}

	void MyserviceFirst::Run()
	{
		ReportStatus(SERVICE_START_PENDING, 2000);
		std::fstream file;
		file.open("C:\\Test.txt", std::fstream::out | std::fstream::app);

		ReportStatus(SERVICE_RUNNING);

		while(GetFlag() != STOP)
		{
			if(GetFlag() != PAUSE)
			{
				file << "This is some test data to write to the file" << std::endl;
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
		file.close();
		ReportStatus(SERVICE_STOPPED);
	}

	void MyserviceFirst::OnStop()
	{
		if(GetFlag() == PAUSE)
		{
			SetFlag(STOP);
			Lock lock(m_mutexNotify);
			m_cond.notify_one();
		}
		SetFlag(STOP);
	}

	void MyserviceFirst::OnPause()
	{
		SetFlag(PAUSE);
	}

	void MyserviceFirst::OnStart()
	{
		SetFlag(START);
		Lock lock(m_mutexNotify);
		m_cond.notify_one();
	}

	void MyserviceFirst::SetFlag(unsigned value)
	{
		Lock lock(m_mutex);
		m_flag = value;
	}

	unsigned MyserviceFirst::GetFlag() const
	{
		Lock lock(m_mutex);
		return m_flag;
	}
}