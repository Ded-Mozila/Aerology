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
	void Write_file_TTAA(int period , const string _file , fstream & file );												// Вывод данных

	void TimePeriod(const int newDate, const int NewTime, int& Date, int& Time );

	void TTBB(char * code , int key );

	void TTCC(char * code);
												// Вывод данных
	
	void TTDD(char * code);
	WIND_SHIFT WindShift( int GGPPP );										// Îïðåäåëåíèå ñäâèãà âåòðà
	char * deleteEndl( char* code );  										// delete '\r\n' in string code
	char * MaxWind ( char* code, surfaceWind & new_max_wind,  int GGPPP );	// Îïðåäåëåíèå ìàêñèìàëüíîãî âåòðà

	char * NumberHeight ( char * code, NUMBER_HEIGHT & new_height, int & i);	// Îïðåäåëåíèå íîìåðà óðîëâíÿ è âûñîòû íàä óðîâíåì ìîðÿ

	char * DistrictStation ( char * code, int & new_number);	// Îïðåäåëåíèå Íîìåðà ðàéîíà è ñòàíöèè

	char * DateTime ( char * code, bool& node, DATA_TIME & new_data_tim );				// Îïðåäåëåíèå Äàòû è âðÿìû çàïóñêà çîíäà + îïðåäåëåíèå èíäåíòèôîêàòîðà ïîñëåäíåé èçîáàðè÷åñêîé ïîâåðõíîñòè(ÒÒÀÀ) ëèáî òèïà îáîðóäîâàíèÿ(ÒÒÂÂ)

	char * Pressure ( char * code, int & press );							// Çíà÷åíèå äàâëåíèÿ

	char * ReturnSurface ( char * code , surface & info , int& i);					// Çàáèâàåò äàííûìè î òåìïåðàòóðå è î âåòðå  type surfase

	char * TempDewpoint ( char * code, TEMP_DEWPOINT & new_info_temp, int& i );		// Äàåò äàííûå îò òåìïåðàòóðå è äåôåöèòå ðîñû âîçâðàùàåò êîä ñòðîêè (ñòðîêà ñ êîäîì, ñòðóêòóðà â êîòîðóþ ñîõðàíèòüñÿ äàííûå î òåìïåðàòóðå è äåôèöèòå ðîñû

	char * Wind ( char * code, WIND & new_wind ,bool node, int& i);							// Çíà÷åíèå âåòðà

	double Temp ( bool presence_dewpoint, const int TTTDD );				// Âûäåëåíèå òåìïåðàòóðû

 	double DefinitionTenthTemp ( int m );									// Îïðåäåëåíèå äàííûõ î òåìïåðàòóðå

	double DewPoint ( int TTTDD );											// Äåôèöèò ðîñû

	void OutSpecialPointWind(void);	//Вывод данных специальных тоек по ветру
	void OutFileListWind(TTBB_Database time_data, fstream & file, int key);
	void Write_file_Wind( int period, fstream & file , int key);
	//Ôóíêöèè çàïèñè äàííûõ â ôàéë.
	//////////////////////////////////////////////////////////////////////////

	void WriteFile( const string file);										//ñîðòèðîâêà äàííûõ äëÿ çàïèñè Êîäîâ ÒÒAA, TTBB

	void OutCodTTAA( const string _file );									//Ïðîöåññ îòêðûòèÿ ôàéëà äëÿ çàïèñè êîäà ÒÒÀÀ
	void Write_file_TTCC( int period, const string _file, fstream & file );	
	void OutCodTTCC( const string _file );									//Ïðîöåññ îòêðûòèÿ ôàéëà äëÿ çàïèñè êîäà 
	
	void Write_file_TTBB( int period, fstream & file , int key );												// Вывод данных
	void OutFileListTTBB( TTBB_Database j, fstream& file, int key );

	void OutCodTTBB( int key );									//Ïðîöåññ îòêðûòèÿ ôàéëà äëÿ çàïèñè êîäà ÒÒÂÂ

	string StrNameFile(local_time st, int time, string _file );		//Ôîðìèðóåò ñòîðîêó äåðåêòîðèè äëÿ îòêðûòèÿ ôàéëà


	void WriteLand(const TTAA_Database time_data, fstream & file);			// Ôóíêöèÿ çàïèñè äàííûõ íà óðîâíè çåìëè
//новая версия
	bool WriteStandateSurfase( const Station time_station, fstream & file , bool StopProcesingLevels);// (âîçâðàùàåò  êëþ÷ íà îñòàíîâêó) Ôóíêöèÿ çàïèñè ñòàíäàðòíûõ èçîáàðè÷åñêèõ ïîâåðõíîñòåé ïîâåðõíîñòåé

	bool WriteStandateSurfase_TTCC( const TTCC_Database time_data, fstream & file , bool StopProcesingLevels);

	void WriteSpecialPoints(const TTAA_Database time_data, fstream & file);	// Ôóíêöèÿ çàïèñè ñïåöèàëüíûõ ïóíêòîâ êîäà ÒÒÂÂ

	int DayPeriod(int time , int day);

	int LastDayPrecedingMonth(void);


	void ViewTimePeriod_00_(fstream & file);


	void ViewTimePeriod_12_(fstream & file);

	void finishApp(fstream & file );
};

#endif // !WRITEFILE_H
