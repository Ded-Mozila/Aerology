#include "header.h"

local_time::local_time() //Конструктор по умолчанию
{
	time_t rawtime;
	struct tm * info;
	time(&rawtime);
	info = localtime (&rawtime);
	wYear = info->tm_year+=1900;
	wMonth = info->tm_mon+1;
	wDay = info->tm_mday;
	whour = info->tm_hour;
	wmin = info->tm_min;
	wsec = info->tm_sec;

}
local_time::~local_time() //Деструктор по умолчанию
{}
local_time::local_time(long year, long month, long day, long hour)
{
	wYear = year;
	wMonth = month;
	wDay = day;
	whour = hour;
}
local_time local_time::Yesterday()
{
	local_time yesterday;
	if (wDay == 1)// Если первое число
	{
		if (wMonth == 1) //Январь
		{
			yesterday.wDay = 31;
			yesterday.wMonth = 12;
			yesterday.wYear = wYear - 1;
		}
		else//Если обычный месяц
		{
			yesterday.wMonth = wMonth - 1;
			yesterday.wYear = wYear;
			// Получение значения высокосного года
			int monline[12]={31,28,31,30,31,30,31,31,30,31,30,31};
			if (LeapYear(wYear)) monline[1]=29;
			yesterday.wDay = monline[yesterday.wMonth-1];
		}
	}
	else//тот же месяц день назад
	{
		yesterday.wDay = wDay - 1;
		yesterday.wMonth = wMonth;
		yesterday.wYear = wYear;
	}
	return yesterday;
}
bool LeapYear( int year )
{
	bool Leap = false;
	if (fmodl(year,4)==0)
	{
		if (fmodl(year,100)==0)
		{
			if (fmodl(year,400)==0) Leap=1;
			else Leap=0;
		}else Leap=1;
	}else Leap=0;
	return Leap;
};