#ifndef DB_STRUCTURE_TTBB_H
#define DB_STRUCTURE_TTBB_H 
#include "General_data.h"

//////////////////////////////////////////////////////////////////////////
class Base
{
public:
	bool information;					// Идентификатор существования данных  
	int	number;							// Отличительные цифры, указывающие, что далее передаются данные  у поверхности земли
	int pressure;						// Давление на уровне
	Base(){};
	Base(const string& str)
	{
		int GGPPP = atoi(str.c_str());
		if (str[2] == '/') 
		{
			number = GGPPP ;
			pressure = 999;
		} 
		else
		{
			number = GGPPP / 1000;
			pressure = GGPPP % 1000; 
		}
		information = true;	
	};
};
class Wind_Base: public Base
{
public:
	WIND wind;							// Данные по ветру
	Wind_Base(){};
	Wind_Base(const string& str):Base(str){};	
};

//////////////////////////////////////////////////////////////////////////

class Temp_Base: public Base
{
public:
	TEMP_DEWPOINT info_temp;			// Информация о данных температуры и дефекта точки росы
	Temp_Base(){};
	Temp_Base(const string& str):Base(str){};	
};

//////////////////////////////////////////////////////////////////////////

class TTBB_Database:public TTXX_Database 
{
public:
	CloudInfo cloud;					// Информация о облочности 
	list<Temp_Base> level;				// Уровни по температуре 
	list<Wind_Base> level_wind;			// Уровни по ветру
	TTBB_Database(){};
	TTBB_Database(const string& str,DATA_TIME m,int n):TTXX_Database(str,m,n){};
	bool operator< (const TTBB_Database & right) 
	{
		int m = right.number;
		int n = number;
		return n<m;
	} ;
};
#endif // !DB_STRUCTURE_TTBB_H