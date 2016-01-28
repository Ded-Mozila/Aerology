#include "header.h"
#ifndef SETTINGS_H
#define SETTINGS_H 
#define DEFAULT_MODE S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH

class Settings
{
public:
    string outDirectory;		//Исходный файл с кодами 
	string inDirectory;			//Дириктория записи декодируемой информации
	string dataDirectory;		//Директория записи кодов которые разкодировались
	list<int> stations;			//Список станция которых нужно декодировать
	fstream fileSettings;		//Поток файла с настройками
	Settings();
	~Settings();
				
	string ToDayData(char * address);							// Формирование пути YYYY/MM 
	string WhatMonth(const long month);							// Формирование ММ
	void init(void);											// Чтение файла с настройками	
	void InitTimePeriodSart(char * dir);						// Инициализации времени запуска программы
};


bool mkdirp(const char* path, mode_t mode = DEFAULT_MODE);		// Создание дериктории по исходному пити
#endif // !SETTINGS_H
