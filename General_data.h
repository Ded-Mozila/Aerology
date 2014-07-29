#ifndef GENERAL_DATA_H
#define GENERAL_DATA_H 


struct WIND
{
	int wind_direction;					// Направление ветра
	int wind_speed;						// Скорость ветра;
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
	int below_speed;					// Абсолютная величина векторной разности в м /с на  уровне максимума и ветра на 1 км ниже него
};

struct NUMBER_HEIGHT
{
	int number;							// Номер уровня
	int height;							// Высота над уровнем моря
};

#endif // !GENERAL_DATA_H