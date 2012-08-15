#include "stdafx.h"
#include "MyServiceSecond.h"


namespace service
{
	MyserviceSecond::MyserviceSecond()
		: MyserviceFirst(Name1)
	{
	}

	void MyserviceSecond::Run()
	{
		ReportStatus(SERVICE_START_PENDING, 2000);

		std::fstream file;
		file.open("C:\\Test1.txt", std::fstream::out | std::fstream::app);

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
}