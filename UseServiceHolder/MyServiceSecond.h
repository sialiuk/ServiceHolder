#pragma once

#include "MyServiceFirst.h"

namespace service
{
	static TCHAR Name1[] = L"UseServiceHolder1";

	class MyserviceSecond
		: public MyserviceFirst
	{
	public:
		MyserviceSecond();
		virtual void Run();
	};
}