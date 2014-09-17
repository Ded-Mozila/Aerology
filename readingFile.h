#ifndef READINGFILE_H
#define READINGFILE_H 
#include "WriteFile.h"
#include "settings.h"
class readingFile
{
public:

	Settings app;
	fstream file;								// Исходный файл
	recording_and_write  TTXX;					// Êëàññ ðàçêîäèðîâàíèÿ è çàïèñè
	readingFile(void);							// Конструктор по умолчанию
	~readingFile(void);
	void OpenFile_(void);
	void MadeNameAeroindex( void );				// Создание имени для исходного файла
	void Read( void );							// Функция поиска данных
	void SelectionCipher(void);
	

	// Модуль тестирования с выводом сканируемых данных
	string outFile_name;		//Исходный файл
	string inFile_name;			//Выходной файл
	string dataFile_name;		// название файла + директроия с исходными данными
};

#endif	// !READINGFILE_H 

