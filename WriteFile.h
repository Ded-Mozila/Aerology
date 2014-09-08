#ifndef WRITEFILE_H
#define WRITEFILE_H
#include "header.h"
#include <fstream>
#include <string.h>
#include "DB_structure_TTAA.h"
#include "DB_structure_TTBB.h"
#include "DB_structure_TTCC.h"
using namespace std;
#define STOP 77
class recording_and_write
{
public:

	int data;					// 20140715 ГГГГ_ММ_ДД
	int time_period;			
	char nameFile[256];

	list<DateSurfase_TTAA> data_TTAA;
	list<DateSurfase_TTBB> data_TTBB;
	list<DateSurfase_TTCC> data_TTCC;
	fstream InFile;
	recording_and_write(void);
	~recording_and_write(void);

	void TTAA(char * code);

	void TimePeriod( TTAA_Database &base, DateSurfase_TTAA &new_base_ );
	void TimePeriod(const int newDate, const int NewTime, int& Date, int& Time );
	void TTBB(char * code);
	void TTCC( char * code );

	WIND_SHIFT WindShift( int GGPPP );										// Îïðåäåëåíèå ñäâèãà âåòðà

	char * deleteEndl( char* code );  										// delete '\r\n' in string code
	char * MaxWind ( char * code, surfaceWind & new_max_wind,  int GGPPP );	// Îïðåäåëåíèå ìàêñèìàëüíîãî âåòðà

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

	bool LeapYear( local_time st );

	void OutCodTTBB( const string _file );									//Ïðîöåññ îòêðûòèÿ ôàéëà äëÿ çàïèñè êîäà ÒÒÂÂ

	string StrNameFile(local_time st, int time, int date, string _file );		//Ôîðìèðóåò ñòîðîêó äåðåêòîðèè äëÿ îòêðûòèÿ ôàéëà

	void OutFileListTTBB( list<TTBB_Database>::iterator j, fstream &inFile );

	void WriteLand(const TTAA_Database time_data, fstream & inFile);			// Ôóíêöèÿ çàïèñè äàííûõ íà óðîâíè çåìëè

	bool WriteStandateSurfase( const TTAA_Database time_data, fstream & inFile , bool StopProcesingLevels);// (âîçâðàùàåò  êëþ÷ íà îñòàíîâêó) Ôóíêöèÿ çàïèñè ñòàíäàðòíûõ èçîáàðè÷åñêèõ ïîâåðõíîñòåé ïîâåðõíîñòåé

	bool WriteStandateSurfase_TTCC( const TTCC_Database time_data, fstream & inFile , bool StopProcesingLevels);

	void WriteSpecialPoints(const TTAA_Database time_data, fstream & inFile);	// Ôóíêöèÿ çàïèñè ñïåöèàëüíûõ ïóíêòîâ êîäà ÒÒÂÂ

	int DayPeriod(int time , int day);

	int LastDayPrecedingMonth();


	void ViewTimePeriod_00_(fstream & inFile);


	void ViewTimePeriod_12_(fstream & inFile);

};

#endif // !WRITEFILE_H
