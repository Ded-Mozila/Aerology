#ifndef LOCAL_TIME_H
#define LOCAL_TIME_H
#include <string.h>
#include <time.h>
#include <iostream>
using namespace std;
class local_time
{
public:
	long wDay;
 	long wMonth;
 	long wYear;
 	long whour;
 	~local_time()
 	{};
 	
 	local_time()
	{
		time_t rawtime;
   		struct tm * info;
   		time(&rawtime);
   		info = localtime (&rawtime);
   		wYear = info->tm_year+=1900;
   		wMonth = info->tm_mon+1;
   		wDay = info->tm_mday;
   		whour = info->tm_hour;
	
	};
	
};
#endif //! LOCAL_TIME_H
