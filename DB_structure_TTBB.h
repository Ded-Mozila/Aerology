#ifndef DB_STRUCTURE_TTBB_H
#define DB_STRUCTURE_TTBB_H 

#include <list>
#include "General_data.h"
using namespace std;

//////////////////////////////////////////////////////////////////////////

struct Wind_Base 
{
	bool information;					// Идентификатор существования данных  
	int	number;							// Отличительные цыфры, указывающие, что далее передаются данные  у поверхности земли
	int pressure;						// Давление на уровне
	WIND wind;							// Данные по ветру
};

//////////////////////////////////////////////////////////////////////////

struct Temp_Base
{
	bool information;					// Идентификатор существования данных
	int	number;							// 
	int pressure;						// Давление на уровне
	TEMP_DEWPOINT info_temp;			// Информация о данных температуры и дефекта точки росы
	
};

//////////////////////////////////////////////////////////////////////////

class TTBB_Database
{
public:
	bool information;					// Отсутствие информации
	DATA_TIME memory;					// Данные о времени и дате запуска и + индентификатор
	int number;			// Данные о територии запуска зонда и станции 
	list<Temp_Base> level;				// Уровни по температуре 
	list< list<Wind_Base> >level_wind;	// Уровни по ветру
	bool operator< (const TTBB_Database & right) 
	{
		int m = right.number;
		int n = number;
		return n<m;
	} ;
};

class DateSurfase_TTBB
{
public:
	int date;
	int time;
	list<TTBB_Database> data_;
	bool operator< (const DateSurfase_TTBB & right) 
	{
		return date < right.date;
	} ;
};
#endif // !DB_STRUCTURE_TTBB_H