#ifndef DB_STRUCTURE_TTAA_H
#define DB_STRUCTURE_TTAA_H 
#include "General_data.h"

class TTCC_Database:public TTXX_Database   // класс для хранения и обработки данных из кода TTCC
{
public:
	list<standardSurface> level;        // Стандартные уровни 70 50 40 30 20 10
	list<surface> tropopause;           // Сведения о тропопаузе 88
	list<surfaceWind> max_wind;         // Сведения о максимальном ветре 77 66 
	InfoRadiationAmendment radioData;	// Сведения о радиационной поправке
	TTCC_Database(){};
	TTCC_Database(const string& str,DATA_TIME m,int n):TTXX_Database(str,m,n){};
};

class TTAA_Database :public TTCC_Database						// класс для хранения и обработки данных из кода TTAA
{
public:
	TTAA_Database(){};
	TTAA_Database(const string& str,DATA_TIME m,int n):TTCC_Database(str,m,n){};
	surface land_surface;				// Поверхность земли 99 
};
#endif // !DB_STRUCTURE_TTAA_H
