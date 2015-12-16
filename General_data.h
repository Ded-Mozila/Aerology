#ifndef GENERAL_DATA_H
#define GENERAL_DATA_H 
#include <list>
#include <cstring>
using namespace std;



class WIND
{
public:
	int wind_direction;                 // Направление ветра
	double wind_speed;                  // Скорость ветра;
	WIND()
	{
		wind_direction = 999;
		wind_speed = 999;
	}
};

struct DATA_TIME
{
	int date;                           // Дата запуска
	int time;                           // Время запуска
	char id_surface;                    // Тип используемого измерительного оборудования
};

struct DISTRICT_STATION
{
	int district_number;                // Номер большого района
	int station_number;                 // Номер гидрометеорологической станции
};

class TEMP_DEWPOINT
{
public:
	double temp;                        // Значение температуры
	double dewpoint;                    // Дефицит точки росы
	TEMP_DEWPOINT()
	{
		temp = 999;
		dewpoint = 999;
	}
};

class  WIND_SHIFT                       // Сведения о вертикальном сдвиге ветра
{
public:
	bool information;
	int up_speed;                       // Абсолютная величина векторной разности в м/с на  уровне максимума и ветра на 1 км выше него
	int below_speed;                    // Абсолютная величина векторной разности в м/с на  уровне максимума и ветра на 1 км ниже него
	WIND_SHIFT()
	{
		information = false;
		up_speed = 999;
		below_speed = 999;
	}
};

class NUMBER_HEIGHT
{
public:
	int number;                         // Номер уровня
	int height;                         // Высота над уровнем моря
	NUMBER_HEIGHT(){
		number = 999;
		height = 999;
	}
};


class surface                           // Все стандартыне данные для всех повехностей
{
public:
	bool wind_node;                     // В телеграмме ветер передается в узлах
	bool information;                   // Идентификатор существования данных  
	TEMP_DEWPOINT info_temp;            // Информация о данных температуры и дефекта точки росы
	WIND wind;                          // Данные по ветру
	int pressure;                       // Давление на уровне 
	surface()
	{
		wind_node = false;
		information = false;
		pressure = -1;
	}

};


class standardSurface                  // Стандартные уровни 
{
public:
	NUMBER_HEIGHT height;               // данные о высоте на уровне
	surface data;                       // Данные о уровне
	standardSurface(){};
};

struct surfaceWind                      // Данные  с максимальной скоростью ветра 77 66
{
	int point;                          // 66 или 77
	bool information;                   // Отсутствие информации
	surface data;                       // Данные о уровне
	WIND_SHIFT shift;                   // Сведения о вертикальном сдвиге ветра
};

class InfoRadiationAmendment            //Сведения о радиационной поправке
{
public:
	bool informationRadia;
	int s;
	int rr;
	int ss;
	//8GGgg
	bool informationTime;
	int GG;
	int gg;
	InfoRadiationAmendment()
	{
		informationRadia = false;
		s = 999;
		rr = 999;
		ss = 999;
		informationTime = false;
		GG = 999;
		gg = 999;
	}
	void getTime(string str)
	{
		if(str[0] == '8')
		{
			informationTime = true;
			GG = atoi(str.substr(1,2).c_str());
			gg = atoi(str.substr(3,2).c_str());
		}
	};
	void getRadio(string str)
	{
		informationRadia = true;
		s = str[0];
		rr = atoi(str.substr(1,2).c_str());
		ss = atoi(str.substr(3,2).c_str());
	};
};
class CloudInfo						//ИНформация о оболочность на станций
{
public:
	int count;						//Количество облаков  Сл и См
	int typeCl;						//Тип облаков Сл
	int h;							//Высота нижней границы самых низких облаков
	int typeCm;						//Тип облаков среднего яруса и слоисто-дождевые облака
	int typeCh;						//Тип облака верхнего яруса	
	void getCloud(string str)
	{
		count = str[0];
		typeCl = str[1];
		h = str[2];
		typeCm = str[3];
		typeCh = str[4];
	};
};
class TTXX_Database						//Стандартный класс на основе кторого осовывають вся коды 
{
public:
	bool information;					// Отсутствие информации
	string code_;						// Исходный шифрованный код
	DATA_TIME memory;					// Данные о времени и дате запуска и + индентификатор
	int number;							// Данные о територии запуска зонда и станции 
	InfoRadiationAmendment radioData;
	TTXX_Database(){};
	TTXX_Database(const string& str,DATA_TIME m,int n):code_(str),memory(m),number(n),information(true){};
};

#endif // !GENERAL_DATA_H
