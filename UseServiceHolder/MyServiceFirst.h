#pragma once

#include <fstream>
#include <boost\thread\mutex.hpp>
#include <boost\thread\condition_variable.hpp>
#include "..\ServiceHolder\ServiceBase.h"

 
namespace service
{
	
	extern TCHAR Name[];

	class MyserviceFirst
		: public sch::ServiceBase
	{
	protected:
		enum
		{
			START,
			STOP,
			PAUSE
		};
	typedef boost::unique_lock<boost::mutex> Lock;
	public:
		MyserviceFirst();
	protected:
		MyserviceFirst(TCHAR* name);
	public:
		virtual void Run();
		virtual void OnStop();
		virtual void OnPause();
		virtual void OnStart();
	protected:
		void SetFlag(unsigned value);
		unsigned GetFlag() const;
	protected:
		unsigned m_flag;
		mutable boost::mutex m_mutex;
		mutable boost::mutex m_mutexNotify;
		boost::condition_variable m_cond;
	};
}