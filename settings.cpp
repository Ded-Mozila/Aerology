#include "settings.h"
#include <errno.h>

Settings::Settings()
{
	fileSettings.open( "settings.txt", ios_base::in );
	if(!fileSettings)
	{
		perror("Error open file settings.txt!");
		cin.get();
		exit(1);
	}
	else
	{
		init();
	}
}

Settings::~Settings()
{
}

void Settings::init()
{
	char str[256];
	char strDirectory[256];
	while (!fileSettings.getline ( str, 256, '#' ).eof())
	{
		char getStr;
		fileSettings.get ( getStr );
		if ( getStr == 'O' || getStr == 'I' )
		{
			fileSettings.getline ( str, 256, '=' );
			switch (getStr)
			{
			case 'O':	
				{
					char dir[256];
					fileSettings.getline ( dir, 256, '=' );
					outDirectory =  dir;
					break;
				}
			case 'I':
				{
					char dir[256];
					fileSettings.getline ( dir , 256, '=' );
					inDirectory = ToDayData(dir);
					mkdirp(inDirectory.c_str());
					break;
				}
			default: break;
			}
		}
	}
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
		"Январь",
		"Февраль",
		"Март",
		"Апрель",
		"Май",
		"Июнь",
		"Июль",
		"Август",
		"Сентябрь",
		"Октябрь",
		"Ноябрь",
		"Декабрь"
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