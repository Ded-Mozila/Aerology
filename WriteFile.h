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

	void TTBB(char * code);
	void Write_file_TTBB( int period, const string _file, fstream & file );												// Вывод данных
	void OutFileListTTBB( TTBB_Database j, fstream& file );

	void TTCC(char * code);
	void Write_file_TTCC( int period, const string _file, fstream & file );												// Вывод данных
	
	WIND_SHIFT WindShift( int GGPPP );										// Îïðåäåëåíèå ñäâèãà âåòðà
	char * deleteEndl( char* code );  										// delete '\r\n' in string code
	char * MaxWind ( char* code, surfaceWind & new_max_wind,  int GGPPP );	// Îïðåäåëåíèå ìàêñèìàëüíîãî âåòðà

	char * NumberHeight ( char * code, NUMBER_HEIGHT & new_height, int & i);	// Îïðåäåëåíèå íîìåðà óðîëâíÿ è âûñîòû íàä óðîâíåì ìîðÿ

	char * DistrictStation ( char * code, int & new_number);	// Îïðåäåëåíèå Íîìåðà ðàéîíà è ñòàíöèè

	char * DateTime ( char * code, DATA_TIME & new_data_tim );				// Îïðåäåëåíèå Äàòû è âðÿìû çàïóñêà çîíäà + îïðåäåëåíèå èíäåíòèôîêàòîðà ïîñëåäíåé èçîáàðè÷åñêîé ïîâåðõíîñòè(ÒÒÀÀ) ëèáî òèïà îáîðóäîâàíèÿ(ÒÒÂÂ)

	char * Pressure ( char * code, int & press );							// Çíà÷åíèå äàâëåíèÿ

	char * ReturnSurface ( char * code , surface & info );					// Çàáèâàåò äàííûìè î òåìïåðàòóðå è î âåòðå  type surfase

	char * TempDewpoint ( char * code, TEMP_DEWPOINT & new_info_temp );		// Äàåò äàííûå îò òåìïåðàòóðå è äåôåöèòå ðîñû âîçâðàùàåò êîä ñòðîêè (ñòðîêà ñ êîäîì, ñòðóêòóðà â êîòîðóþ ñîõðàíèòüñÿ äàííûå î òåìïåðàòóðå è äåôèöèòå ðîñû

	char * Wind ( char * code, WIND & new_wind );							// Çíà÷åíèå âåòðà

	double Temp ( bool presence_dewpoint, const int TTTDD );				// Âûäåëåíèå òåìïåðàòóðû

	double DefinitionTenthTemp ( int m );									// Îïðåäåëåíèå äàííûõ î òåìïåðàòóðå

	double DewPoint ( int TTTDD );											// Äåôèöèò ðîñû


	//Ôóíêöèè çàïèñè äàííûõ â ôàéë.
	//////////////////////////////////////////////////////////////////////////

	void WriteFile( const string file);										//ñîðòèðîâêà äàííûõ äëÿ çàïèñè Êîäîâ ÒÒAA, TTBB

	void OutCodTTAA( const string _file );									//Ïðîöåññ îòêðûòèÿ ôàéëà äëÿ çàïèñè êîäà ÒÒÀÀ

	void OutCodTTCC( const string _file );									//Ïðîöåññ îòêðûòèÿ ôàéëà äëÿ çàïèñè êîäà 
	
	void OutCodTTBB( const string _file );									//Ïðîöåññ îòêðûòèÿ ôàéëà äëÿ çàïèñè êîäà ÒÒÂÂ

	string StrNameFile(local_time st, int time, string _file );		//Ôîðìèðóåò ñòîðîêó äåðåêòîðèè äëÿ îòêðûòèÿ ôàéëà


	void WriteLand(const TTAA_Database time_data, fstream & file);			// Ôóíêöèÿ çàïèñè äàííûõ íà óðîâíè çåìëè

	bool WriteStandateSurfase( const TTAA_Database time_data, fstream & file , bool StopProcesingLevels);// (âîçâðàùàåò  êëþ÷ íà îñòàíîâêó) Ôóíêöèÿ çàïèñè ñòàíäàðòíûõ èçîáàðè÷åñêèõ ïîâåðõíîñòåé ïîâåðõíîñòåé

	bool WriteStandateSurfase_TTCC( const TTCC_Database time_data, fstream & file , bool StopProcesingLevels);

	void WriteSpecialPoints(const TTAA_Database time_data, fstream & file);	// Ôóíêöèÿ çàïèñè ñïåöèàëüíûõ ïóíêòîâ êîäà ÒÒÂÂ

	int DayPeriod(int time , int day);

	int LastDayPrecedingMonth(void);


	void ViewTimePeriod_00_(fstream & file);


	void ViewTimePeriod_12_(fstream & file);

};

#endif // !WRITEFILE_H
