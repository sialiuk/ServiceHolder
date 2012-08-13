// UseServiceHolder.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MyService.h"
#include "MyService1.h"

int _tmain(int argc, _TCHAR* argv[])
{
	sch::ServiceGroupManager serviceGroupe;
	serviceGroupe.AddService(serviceName, sch::ServiceBase::ServiceFunction<Myservice>);
	serviceGroupe.AddService(serviceName1, sch::ServiceBase::ServiceFunction<Myservice1>);
	
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

