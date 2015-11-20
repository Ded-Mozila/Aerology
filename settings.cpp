#include "settings.h"
#include <errno.h>


Settings::Settings()
{
	fileSettings.open( "settings.txt", ios_base::in );
	if(!fileSettings)
	{
		perror("Error open file settings.txt!");
		exit(1);
	}
	else
	{
		init();
	}
	fileSettings.close();
}

Settings::~Settings()
{
}

void Settings::init()
{
	char dir1[256]; //Исходный файл с кодами 
	fileSettings.getline ( dir1, 256, ';' );
	outDirectory =  dir1;
	char dir2[256]; //Дериктория записи декодируемой информации
	fileSettings.getline ( dir2, 256, ';' );
	inDirectory = ToDayData(dir2);
	mkdirp(inDirectory.c_str());
	char dir3[256];  //Директория записи кодов которые разкодировались
	fileSettings.getline ( dir3, 256, ';' );
	dataDirectory = ToDayData(dir3);
	mkdirp(dataDirectory.c_str());
	char stat[256]; // Промежуточная страка для станций
	fileSettings.getline ( stat, 256, '\n');
	while(!fileSettings.getline ( stat, 256, '\n').eof())
	{
		stations.push_back(atoi(stat));
	}
	stations.push_back(atoi(stat));
}


string Settings::ToDayData( char * address )
{
	local_time timeInfo; //время
	stringstream new_addres;
	new_addres << address <<  "/" << timeInfo.wYear <<  "/" << WhatMonth(timeInfo.wMonth);
	string AddresStr;
	new_addres >> AddresStr;
	new_addres.clear();
	return AddresStr;
}


string Settings::WhatMonth( const long month )
{
	string ListMonths[12] = {
		"01",
		"02",
		"03",
		"04",
		"05",
		"06",
		"07",
		"08",
		"09",
		"10",
		"11",
		"12"
	};
	return ListMonths[month-1];
}


bool mkdirp(const char* path, mode_t mode) //Создание дерева директории
{
	// const cast for hack
	char* p = const_cast<char*>(path);
	// Do mkdir for each slash until end of string or error
	while (*p != '\0') 
	{
		// Skip first character
		p++;
		// Find first slash or end
		while(*p != '\0' && *p != '/') p++;

		// Remember value from p
		char v = *p;

		// Write end of string at p
		*p = '\0';

		// Create folder from path to '\0' inserted at p
		if(mkdir(path, mode) == -1 && errno != EEXIST) 
		{
			*p = v;
			return false;
		}
		// Restore path to it's former glory
		*p = v;
	}
	return true;
}