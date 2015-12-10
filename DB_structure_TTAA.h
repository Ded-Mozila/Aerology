#ifndef DB_STRUCTURE_TTAA_H
#define DB_STRUCTURE_TTAA_H 
#include <list>
#include <cstring>
#include "General_data.h"
using namespace std;
// Сохраню номер станции  потом буду проверять 
class TTAA_Database						// класс для хранения и обработки данных из кода TTAA
{
public:
	bool information;					// Отсутствие информации
	DATA_TIME memory;					// Данные о времени и дате запуска и + индентификатор
	int number;							// Данные о територии запуска зонда и станции 
	surface land_surface;				// Поверхность земли 99 
	list<standardSurface> level ;		// Стандартные уровни 00 92 85 70 50 40 30 25 20 15 10
	string code_;						// Исходный шифрованный код
	list<surface> tropopause;           // Сведения о тропопаузе 88
	list<surfaceWind> max_wind;         // Сведения о максимальном ветре 77 66 
	InfoRadiationAmendment radioData;	// Сведения о радиационной поправке
};
#endif // !DB_STRUCTURE_TTAA_H
