#ifndef GENERAL_DATA_H
#define GENERAL_DATA_H 




struct WIND
{
	int wind_direction;					// Направление ветра
	double wind_speed;						// Скорость ветра;
};

struct DATA_TIME
{
	int date;							// Дата запуска
	int time;							// Время запуска
	char id_surface;					// Тип используемого измерительного оборудования
};

struct DISTRICT_STATION
{
	int district_number;				// Номер большого района
	int station_number;					// Номер гидрометеорологической станции
};

struct TEMP_DEWPOINT
{
	double temp;						// Значение температуры
	double dewpoint;					// Дефицит точки росы
};

struct  WIND_SHIFT					// Сведения о вертикальном сдвиге ветра
{
	int up_speed;						// Абсолютная величина векторной разности в м/с на  уровне максимума и ветра на 1 км выше него
	int below_speed;					// Абсолютная величина векторной разности в м/с на  уровне максимума и ветра на 1 км ниже него
};

struct NUMBER_HEIGHT
{
	int number;							// Номер уровня
	int height;							// Высота над уровнем моря
};


class surface                       // Все стандартыне данные для всех повехностей
{
public:
	bool wind_node;						// В телеграмме ветер передается в узлах
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


struct standardSurface                  // Стандартные уровни 
{
	NUMBER_HEIGHT height;               // данные о высоте на уровне
	surface data;                       // Данные о уровне
};

struct surfaceWind                      // Данные  с максимальной скоростью ветра 77 66
{
	int point;                          // 66 или 77
	bool information;                   // Отсутствие информации
	surface data;                       // Данные о уровне
};


#endif // !GENERAL_DATA_H