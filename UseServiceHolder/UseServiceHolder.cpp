// UseServiceHolder.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "MyServiceSecond.h"
#include "..\ServiceHolder\ServiceGroupManager.h"

using namespace service;
int _tmain(int argc, _TCHAR* argv[])
{
	sch::ServiceGroupManager serviceGroupe;
	serviceGroupe.AddService(Name, sch::ServiceBase::ServiceFunction<MyserviceFirst>);
	serviceGroupe.AddService(Name1, sch::ServiceBase::ServiceFunction<MyserviceSecond>);
	
	if( lstrcmpi( argv[1], TEXT("install")) == 0 )
    {
		serviceGroupe.Install(SERVICE_DEMAND_START);
        return 0;
    }
	
	if( lstrcmpi( argv[1], TEXT("remove")) == 0 )
    {
		serviceGroupe.Remove();
        return 0;
    }

	serviceGroupe.StartServiceProcess();

	return 0;
}

