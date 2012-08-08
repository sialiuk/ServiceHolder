// UseServiceHolder.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "IOCP.h"
#include "..\ServiceHolder\Service.h"
#include "..\ServiceHolder\ServiceStatus.h"

wchar_t serviceName[] = L"UseServiceHolder";
sch::ServiceStatus serviceSt;

DWORD WINAPI HandlerEx(DWORD control, DWORD eventType, PVOID eventDate, PVOID context)
{
	DWORD dwReturn = ERROR_CALL_NOT_IMPLEMENTED;
	BOOL fPostControlToServiceThread = FALSE;

	switch (control) 
	{
	case SERVICE_CONTROL_STOP:
	case SERVICE_CONTROL_SHUTDOWN:
		serviceSt.SetUltimateStatus(SERVICE_STOPPED, 2000);
		fPostControlToServiceThread = TRUE;
		break;

	case SERVICE_CONTROL_PAUSE:
		serviceSt.SetUltimateStatus(SERVICE_PAUSED, 2000);
		fPostControlToServiceThread = TRUE;
		break;

	case SERVICE_CONTROL_CONTINUE:
		serviceSt.SetUltimateStatus(SERVICE_RUNNING, 2000);
		fPostControlToServiceThread = TRUE;
		break;

	case SERVICE_CONTROL_INTERROGATE:
		serviceSt.ReportStatus();
		break;

	case SERVICE_CONTROL_PARAMCHANGE:
		break;

	case SERVICE_CONTROL_DEVICEEVENT:
	case SERVICE_CONTROL_HARDWAREPROFILECHANGE:
	case SERVICE_CONTROL_POWEREVENT:
		break;
   }

	if (fPostControlToServiceThread) 
	{
		CIOCP* piocp = (CIOCP*) context;
		piocp->PostStatus(0, control);
		dwReturn = NO_ERROR;
	}

   return(dwReturn);
}

void WINAPI ServiceMain(DWORD dwArgc, PTSTR* pszArgv)
{
	ULONG_PTR CompKey;
	DWORD dwControl = SERVICE_CONTROL_CONTINUE;
	HANDLE hFile;
	OVERLAPPED o; 
	OVERLAPPED* po  = NULL;
	DWORD dwNumBytes;

	CIOCP iocp(0);
	//serviceSt.Initialize(serviceName, HandlerEx, (PVOID) &iocp);
	serviceSt.AcceptControls(SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE);

	do
	{
		switch(dwControl)
		{
		case SERVICE_CONTROL_CONTINUE:
			hFile = CreateFile(L"D:\\test.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL
								, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			WriteFile(hFile,"Service works\n", 14, &dwNumBytes, NULL);
			break;

		case SERVICE_CONTROL_PAUSE:
			iocp.GetStatus(&CompKey, &dwControl, &po);
			serviceSt.ReportUltimateStatus();
			break;

		case SERVICE_CONTROL_STOP:
			CloseHandle(hFile);
			serviceSt.ReportUltimateStatus();
			break;
		}

		if(serviceSt.dwCurrentState != SERVICE_STOPPED)
		{
			iocp.GetStatus(&CompKey, &dwControl, &po, 10000);
		}

	} while(serviceSt.dwCurrentState != SERVICE_STOPPED);
}


int _tmain(int argc, _TCHAR* argv[])
{

	return 0;
}

