// UseServiceHolder.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <boost\thread\mutex.hpp>
#include <boost\thread\condition_variable.hpp>
#include "..\ServiceHolder\ServiceBase.h"
//#include "..\ServiceHolder\ServiceManager.h"
#include "..\ServiceHolder\ServiceGroupManager.h"

TCHAR serviceName[] = L"UseServiceHolder";
TCHAR serviceName1[] = L"UseServiceHolder123";

namespace
{
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
			, m_flag(START)
		{
		}

		virtual void Run()
		{
			ReportStatus(SERVICE_START_PENDING, 2000);

			std::fstream file;
			file.open("D:\\Test.txt", std::fstream::out | std::fstream::app);

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

	private:
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


	class Myservice1
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
		Myservice1()
			: sch::ServiceBase(serviceName1)
			, m_flag(START)
		{
		}

		virtual void Run()
		{
			ReportStatus(SERVICE_START_PENDING, 2000);

			std::fstream file;
			file.open("D:\\Test1.txt", std::fstream::out | std::fstream::app);

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

	private:
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
}


int _tmain(int argc, _TCHAR* argv[])
{
	sch::ServiceGroupManager serviceGroupe(L"Service Groupe");
	//sch::ServiceManager serviceManager(serviceName, sch::ServiceBase::ServiceFunction<Myservice>);
	if( lstrcmpi( argv[1], TEXT("install")) == 0 )
    {
		serviceGroupe.Install(SERVICE_DEMAND_START);
       // serviceManager.Install(SERVICE_DEMAND_START);
        return 0;
    }
	
	if( lstrcmpi( argv[1], TEXT("remove")) == 0 )
    {
		serviceGroupe.Remove();
		//serviceManager.Remove();
        return 0;
    }
	serviceGroupe.AddService(serviceName, sch::ServiceBase::ServiceFunction<Myservice>);
	serviceGroupe.AddService(serviceName1, sch::ServiceBase::ServiceFunction<Myservice1>);
	serviceGroupe.StartServiceProcess();
	//serviceManager.StartServiceProcess();

	return 0;
}

