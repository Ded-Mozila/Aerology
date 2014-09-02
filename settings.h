
#ifndef SETTINGS_H
#define SETTINGS_H 
#define DEFAULT_MODE S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH
#include "header.h"

class Settings
{
public:
    string outDirectory;
	string inDirectory;
	string dataDirectory;  // директория сохранения исходных данных

	fstream fileSettings;
	Settings();
	~Settings();
				
	string ToDayData(char * address);							// Ïîëó÷àíèå ñòðîêè ñîäåðæàùÿÿ ìåñÿö è ãîä â ôîðìàòå ÃÃÃÃ\Ìåñÿö
	string WhatMonth(const long month);							// Ïåðåâîä è ç ÷èñëà â íàçâàíèå ìåñÿöà
	void init(void);											// èíèöèàëèçàöèÿ ïàðàìåòðîâ ïðîãðàììû	
};


bool mkdirp(const char* path, mode_t mode = DEFAULT_MODE);
#endif // !SETTINGS_H