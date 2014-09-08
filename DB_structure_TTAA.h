#ifndef DB_STRUCTURE_TTAA_H
#define DB_STRUCTURE_TTAA_H 
#include <list>
#include "General_data.h"
using namespace std;

class TTAA_Database                     // класс для хранения и обработки данных из кода TTAA
{
public:
	bool information;                   // Отсутствие информации
	DATA_TIME memory;                   // Данные о времени и дате запуска и + индентификатор
	int number;                         // Данные о територии запуска зонда и станции 
	surface land_surface;               // Поверхность земли 99 
	list<standardSurface> level ;       // Стандартные уровни 00 92 85 70 50 40 30 25 20 15 10
	//list<surface> tropopause;           // Сведения о тропопаузе 88
	//list<surfaceWind> max_wind;         // Сведения о максимальном ветре 77 66 
	//list<WIND_SHIFT> vertical;          // Сведения о вертикальном сдвиге ветра 4
	bool operator== (const TTAA_Database & right) 
	{
		int m = right.number;
		int n = number;
		return n == m;
	} ;
	bool operator!= (const TTAA_Database & right) 
	{
		int m = right.number;
		int n = number;
		return n != m;
	} ;
	bool operator< (const TTAA_Database & right) 
	{
		int m = right.number;
		int n = number;
		return n < m;
	} ;
};

class DateSurfase_TTAA
{
public:
	int date;
	int time;
	list<TTAA_Database> data_;
	void FindReplicaToList();           // Поиск и исключение из списка повторяющюхся и пустых элементов
	bool operator< (const DateSurfase_TTAA & right) 
	{
		return date < right.date;
	} ;
};
#endif // !DB_STRUCTURE_TTAA_H