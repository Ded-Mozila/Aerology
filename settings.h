
#ifndef SETTINGS_H
#define SETTINGS_H 
#define DEFAULT_MODE S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH
#include "header.h"

class Settings
{
public:
    string outDirectory;
	string inDirectory;
	fstream fileSettings;
	Settings();
	~Settings();
				
	string ToDayData(char * address);							// ѕолучание строки содержащ€€ мес€ц и год в формате √√√√\ћес€ц
	string WhatMonth(const long month);							// ѕеревод и з числа в название мес€ца
	void init(void);											// инициализаци€ параметров программы	
};


bool mkdirp(const char* path, mode_t mode = DEFAULT_MODE);
#endif // !SETTINGS_H