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
	long wmin;
	long wsec;
	~local_time();
	local_time();
	local_time(long,long,long,long);//год,месяц,день,час
	local_time Yesterday();	//нахождение вчерашних числа
	
};
bool LeapYear(  int year );
#endif //! LOCAL_TIME_H
