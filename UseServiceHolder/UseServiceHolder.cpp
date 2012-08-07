// UseServiceHolder.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "..\ServiceHolder\Service.h"

int _tmain(int argc, _TCHAR* argv[])
{
	sch::Service service1(L"Myservice");
	service1.Install(SERVICE_DEMAND_START);
	service1.Remove();
	return 0;
}

