#ifndef DB_STRUCTURE_TTAA_H
#define DB_STRUCTURE_TTAA_H 
#include <list>
#include "General_data.h"
using namespace std;
class surface						// Все стандартыне данные для всех повехностей
{
public:
	bool information;					// Идентификатор существования данных  
	TEMP_DEWPOINT info_temp;			// Информация о данных температуры и дефекта точки росы
	WIND wind;							// Данные по ветру
	int pressure;						// Давление на уровне 99
	surface()
	{
		information = false;
		pressure = -1;
	}

};

struct standardSurface					// Стандартные уровни 00 92 85 70 50 40 30 25 20 15 10
{
	NUMBER_HEIGHT height;				// данные о высоте на уровне
	surface data;						// Данные о уровне
};

struct surfaceWind						// Данные  с максимальной скоростью ветра 77 66
{
	int point;							// 66 или 77
	bool information;					// Отсутствие информации
	surface data;						// Данные о уровне
};

class TTAA_Database						// класс для хранения и обработки данных из кода TTAA
{
public:
	bool information;					// Отсутствие информации
	DATA_TIME memory;					// Данные о времени и дате запуска и + индентификатор
	DISTRICT_STATION number;			// Данные о територии запуска зонда и станции 
	surface land_surface;				// Поверхность земли 99 
	list<standardSurface> level ;		// Стандартные уровни 00 92 85 70 50 40 30 25 20 15 10
	list<surface> tropopause;			// Сведения о тропопаузе 88
	list<surfaceWind> max_wind;			// Сведения о максимальном ветре 77 66 
	list<WIND_SHIFT> vertical;			// Сведения о вертикальном сдвиге ветра 4
	bool operator< (const TTAA_Database & right) 
	{
		int m = ((right.number.district_number*1000) + right.number.station_number);
		int n = ((number.district_number*1000) + number.station_number);
		return n < m;
	} ;
};
 class DateSurfase_TTAA
{
public:
	int date;
	int time;
	list<TTAA_Database> data_;
	void FindReplicaToList(); 			// Поиск и исключение из списка повторяющюхся и пустых элементов
	bool operator< (const DateSurfase_TTAA & right) 
	{
		return date < right.date;
	} ;
};
#endif // !DB_STRUCTURE_TTAA_H