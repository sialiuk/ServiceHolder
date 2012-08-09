// UseServiceHolder.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "..\ServiceHolder\ServiceBase.h"
#include "..\ServiceHolder\ServiceManager.h"

TCHAR serviceName[] = L"UseServiceHolder";

class Myservice
	: public sch::ServiceBase
{
public:
	Myservice()
		: sch::ServiceBase(serviceName)
		, m_event(NULL)
	{
		m_event = CreateEvent(NULL, FALSE, FALSE, NULL);
	}

	virtual void Run()
	{
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
		DWORD result;
		while((result = WaitForSingleObject(m_event, 3000)) == WAIT_TIMEOUT)
		{
			WriteFile(hFile,
						DataBuffer,
						sizeof(DataBuffer),
						&numBytes,
						NULL);
			//FlushFileBuffers(hFile);
		}
		if(result == WAIT_OBJECT_0)
		{
			CloseHandle(hFile);
			ReportStatus(SERVICE_STOPPED);
		}
	}

	virtual void OnStop()
	{
		SetEvent(m_event);
	}

private:
	HANDLE m_event;
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

