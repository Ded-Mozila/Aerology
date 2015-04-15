#ifndef WRITEFILE_H
#define WRITEFILE_H
#include <fstream>
#include "header.h"
#include <string.h>

#define STOP 77
class Station 							//Данные о определенной станции
{
public:
	int number;
	bool info;
	TTAA_Database TTAA;					//Данные со станции
	TTBB_Database TTBB;					//
	TTCC_Database TTCC;					// до разборки с TTAA
	TTBB_Database TTDD;
	Station(void)
	{
		number = -1;
		info = false;
	};
	Station(int z):number(z),info(false){};
	~Station(void)
	{
	};
	bool operator< (const Station & right){ //Для сортировки
		return number < right.number;};
	bool operator== (const Station & right){ //Для поиска
		return number == right.number;};
};

class recording_and_write
{
public:
	//новый вид данных
	list<Station> time_00;				// Данные за 00
	list<Station> time_12;				// Данные за 12
	int data;							// 20140715 ГГГГ_ММ_ДД
	recording_and_write(void);
	~recording_and_write(void);
	
	fstream inFile_00;						//Файл с выходной файл
	fstream inFile_12;						//Файл с выходной файл

	void TTAA(char * code);
	void TTBB(char * code , int key );
	void TTCC(char * code);
	void TTDD(char * code);

	void TimePeriod(const int newDate, const int NewTime, int& Date, int& Time );
	// Вывод данных
	void Write_file_TTAA(int period , const string _file , fstream & file );
	
	void Write_file_TTBB( int period, fstream & file , int key );					// Вывод данных TTBB
	
	void Write_file_TTCC( int period, const string _file, fstream & file );	

	WIND_SHIFT WindShift( int GGPPP );												// Определение сдвига ветра
	
	char * deleteEndl( char* code );  												// delete '\r\n' in string code

	char * MaxWind ( char* code, surfaceWind & new_max_wind,  int GGPPP );			// Определение максимального ветра

	char * NumberHeight ( char * code, NUMBER_HEIGHT & new_height, int & i);		// Определение номера уровня и высоты над уровнем моря

	char * DistrictStation ( char * code, int & new_number);						// Определение Номера района и станции

	char * DateTime ( char * code, bool& node, DATA_TIME & new_data_tim );			// Определение Даты и врямы запуска зонда + определение индентифокатора последней изобарической поверхности(ТТАА) либо типа оборудования(ТТВВ)
	
	char * Pressure ( char * code, int & press );									// Значение давления

	char * ReturnSurface ( char * code , surface & info , int& i);					// Забивает данными о температуре и о ветре type surfase

	char * TempDewpoint ( char * code, TEMP_DEWPOINT & new_info_temp, int& i );		// Дает данные от температуре и дефеците росы возвращает код строки (строка с кодом, структура в которую сохраниться данные о температуре и дефиците росы

	char * Wind ( char * code, WIND & new_wind ,bool node, int& i);					// Значение ветра

	double Temp ( bool presence_dewpoint, const int TTTDD );						// Выделение температуры

 	double DefinitionTenthTemp ( int m );											// Определение данных о температуре

	double DewPoint ( int TTTDD );													// Дефицит росы

	void OutSpecialPointWind(void);													//Вывод данных специальных тоек по ветру
	
	void OutFileListWind(TTBB_Database time_data, fstream & file, int key);
	
	void Write_file_Wind( int period, fstream & file , int key);
	
	// Функции записи данных в файл.

	void WriteFile( const string file);										//Cортировка данных для записи Кодов ТТAA, TTBB

	void OutCodTTAA( const string _file );									//Процесс открытия файла для записи кода ТТАА

	void OutCodTTBB( int key );												//Процесс открытия файла для записи кода ТТВВ
	
	void OutCodTTCC( const string _file );									//Процесс открытия файла для записи кода TTCC

	void OutFileListTTBB( TTBB_Database j, fstream& file, int key );

	string StrNameFile(local_time st, int time, string _file );				//Формирует стороку деректории для открытия файла


	void WriteLand(const TTAA_Database time_data, fstream & file);			// Функция записи данных на уровни земли
	//новая версия
	bool WriteStandateSurfase( const Station time_station, fstream & file , bool StopProcesingLevels);// (возвращает ключ на остановку) Функция записи стандартных изобарических поверхностей поверхностей

	bool WriteStandateSurfase_TTCC( const TTCC_Database time_data, fstream & file , bool StopProcesingLevels);

	void WriteSpecialPoints(const TTAA_Database time_data, fstream & file);	// Функция записи специальных пунктов кода ТТВВ
	
	int DayPeriod(int time , int day);

	int LastDayPrecedingMonth(void);


	void ViewTimePeriod_00_(fstream & file);


	void ViewTimePeriod_12_(fstream & file);

	void finishApp(fstream & file );
};

#endif // !WRITEFILE_H
