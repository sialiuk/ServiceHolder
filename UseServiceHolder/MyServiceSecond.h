#pragma once

#include "MyServiceFirst.h"

namespace service
{
	extern TCHAR Name1[];

	class MyserviceSecond
		: public MyserviceFirst
	{
	public:
		MyserviceSecond();
		virtual void Run();
	};
}