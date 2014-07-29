 #include "WriteFile.h"
#include <cstdlib>
#include <string>
#include <cmath>
#include "settings.h"
#include "header.h"
#include <fstream>
#include <string.h>
#include "local_time.h"

void recording_and_write::WriteFile(const string _file )
{
	OutCodTTAA(_file);  // создание файла и запись кода TTAA
	OutCodTTBB(_file);	// запись в созданный уже файла кода TTBB
}

void recording_and_write::OutCodTTAA( const string _file )
{
	local_time st;
	list<DateSurfase_TTAA>::iterator i;
	for ( i = data_TTAA.begin(); i !=data_TTAA.end();++i )
	{
		i->data_.sort();
		//создание 
		string name = StrNameFile( st, i->time, st.wDay, _file );
		fstream inFile( name.c_str() , ios_base::out );
		if(!inFile)
		{
			perror("Error open file!");
			cin.get();
			exit(1);
		}

		//Вывод
		inFile << "Выборка за период: " ;
			 //Сравниваем время для определения дальнейшего алгаритма
		if (11 <= ((time_period/100)%100) <= 13)
			time_period = 12;
			else time_period = 0;

		switch (time_period) //ДДЧЧММ - ДД - день ; ЧЧ - часы ; ММ - минуты
			{
			case 0:// 00 время это промежуток времени от 23:00 - 01:00
				{
					ViewTimePeriod_00_(inFile);
					break;
				}
			case 12:// 12 время этого промежутка от 11:00 до 13:00
				{
					ViewTimePeriod_12_(inFile);
					break;
				}
			default:
				{
					cout << "Fail";
					break;
				}
			}		
			inFile << "\n\n";
		list<TTAA_Database>::iterator J;
		for ( J = i->data_.begin(); J != i->data_.end(); ++J )
		{
			surface DAT = (*J).land_surface;
			double temp = DAT.info_temp.temp;
			inFile <<  (*J).number.district_number;
			if((*J).number.station_number < 100 )inFile<<"0";
			inFile<<(*J).number.station_number <<" " ;
			if ( DAT.information == true /*|| DAT.pressure != NULL || temp != NULL*/ )
			{
				inFile << "Pzem=";
				if ((DAT.pressure) < 1000) inFile << " ";
				inFile << DAT.pressure << " ";
				inFile << "Tzem=";
				if ( temp >= 0 )
				{
					inFile << " ";
				}
				if (fabs(temp) < 10) inFile << " ";
				inFile << temp;
				if ((temp - (int)temp) == 0) inFile << ".0";
				inFile << '\n' ;
			}
			else
			{
				inFile << "Нет данных\n";
			}
		}
		inFile << '\n' <<  "StandartLevels" << "\n";

		for ( J = i->data_.begin(); J != i->data_.end(); ++J )
		{	
			bool controllerEmptyLevels = true;
			controllerEmptyLevels = WriteStandateSurfase( *J , inFile ,controllerEmptyLevels);
		}
		inFile.close();
	}

}

void recording_and_write::ViewTimePeriod_00_(fstream & inFile)
{
	local_time st;
	long day = data%100;
	long month = ((data/100)%100);
	long year = data/10000;
	long old_day;
	long old_month;
	long old_year;
	if (day == 1)
	{
		if (month == 1)
		{
			old_day = 31;
			old_month = 12;
			old_year = year - 1;	
		}
		else
		{
			old_month = month - 1;
			old_year = year;
			// Получение значения высокосного года
			int monline[12]={31,28,31,30,31,30,31,31,30,31,30,31};
			if (LeapYear(st)) monline[1]=29;
			old_day = monline[old_month-1];
		}
	}
	else
	{
		old_day = day - 1;
		old_month = month;
		old_year = year;
	}
	// Начало измерений
	if (old_day < 10) 
		inFile << "0";
	inFile << old_day << ".";
	if (old_month < 10)
		inFile << "0";
	inFile << old_month << "." << old_year  << " 23:00 - ";
	// Начало измерений
	if (day < 10) 
		inFile << "0";
	inFile << day << ".";
	if (month < 10)
		inFile << "0";
	inFile << month << "." << year  << " 01:00\n";

}

void recording_and_write::ViewTimePeriod_12_(fstream & inFile)
{
	long day = data%100;
	long month = ((data/100)%100);
	long year = data/10000;
	for (int h=0 ; h <= 1; h++)
	{
		if(day < 10) 
			inFile << "0";
		inFile << day << ".";
		if(month < 10) 
			inFile << "0";
		inFile << month << "." << year;
		if (h == 0) inFile << " 11:00 - ";
			else inFile << " 13:00";
	}
}

void recording_and_write::WriteLand( const TTAA_Database time_data,fstream & inFile )
{
	surface DAT = time_data.land_surface;
	double temp = DAT.info_temp.temp;
	inFile <<  time_data.number.district_number<<time_data.number.station_number <<" " ;
	if ( DAT.information == true )
	{
		inFile << "Pzem=";
		if ((DAT.pressure) < 1000) inFile << " ";
		inFile << DAT.pressure << " ";
		inFile << "Tzem=";
		if ( temp >= 0 )
		{
			inFile << " ";
		}
		if (fabs(temp) < 10) inFile << " ";
		inFile << temp;
		if ((temp - (int)temp) == 0) inFile << ".0";
		inFile << '\n' ;
	}
	else
	{
		inFile << "Нет данных\n";
	}
}

bool recording_and_write::WriteStandateSurfase( const TTAA_Database time_data, fstream & inFile , bool StopProcesingLevels)
{
	TTAA_Database date = time_data;
	list<standardSurface>::iterator i;
	if (date.information == true)
	{

		for(i = date.level.begin(); i != date.level.end(); ++i )
		{
			int  StandartLevels[11] = {0,92,85,70,50,40,30,25,20,15,10};
			standardSurface new_surfase = *i;
			/*if (new_surfase.data.information == true)// Îòñëåæèâàåò îòñóòñòâèå ñòàíäàðòíûõ óðîâíåé è âñòàâëÿåò çà ìåñòî íèõ ïðîáåëû îòñóòñòâèÿ
			{
				int t;
				cin >>t;*/
				double temp = new_surfase.data.info_temp.temp;				//Òåìïåðàòóðà
				int number = new_surfase.height.number;						//Íîìåð óðîâíÿ
				WIND wind = new_surfase.data.wind;
				if (StopProcesingLevels)
				{
					for (int a = 0; a < 11; a++)
					{
						if (StandartLevels[a] == new_surfase.height.number)
						{
							StopProcesingLevels = false;
							break;
						}
						else
						{
							inFile << "IND="<<  time_data.number.district_number<<time_data.number.station_number <<" ";
							inFile << "P=";
							if (StandartLevels[a] != 0) inFile << " ";
							else inFile << "10";
							inFile << StandartLevels[a];
							if ( StandartLevels[a] == 92 ) inFile << "5";
							else inFile << "0";
							inFile << "================================\n";
						}
					}
				}
				//Íîìåð ñòàíöèè è ðàéîíà
				inFile << "IND="<<  time_data.number.district_number<<time_data.number.station_number <<" ";

				//äàâëåíèå
				inFile << "P=";
				if (number != 0) inFile << " ";
				else inFile << "10";
				inFile << number;
				if ( number == 92 ) inFile << "5";
				else inFile << "0";

				//òåìïåðàòóðà
				inFile << " T=";
				if ( temp == 999 ) inFile << " ";
				if ( temp >= 0 ) inFile << " ";
				if ( fabs(temp) < 10 ) inFile << " ";
				inFile << temp;
				if ( ( temp - (int)temp ) == 0 && temp != 999 ) inFile << ".0";

				//íàïðàâëåíèå âåòðà
				inFile << " d=";
				if ( wind.wind_direction < 100 ) inFile << " ";
				if ( wind.wind_direction < 10 ) inFile << " ";
				inFile << wind.wind_direction;

				//ñêîðîòü âåòðà
				inFile << " f=";
				if ( abs(wind.wind_speed) < 100 )inFile << " ";
				if ( abs(wind.wind_speed) < 10 )inFile << " ";
				inFile << wind.wind_speed;

				//äåôèöèò òî÷êè ðîñû
				inFile << " D=";
				double dewpoint = new_surfase.data.info_temp.dewpoint;
				if ( abs(dewpoint) < 100 )inFile << " ";
				if ( abs(dewpoint) < 10 )inFile << " ";
				inFile << dewpoint;
				if ( ( dewpoint  - (int)dewpoint ) == 0 && dewpoint != 999 ) inFile << ".0";
				if ( dewpoint != 999 ) inFile << "0";
				inFile << "\n" ;
			//}
		}
		inFile << "\n" ;
	}
	return StopProcesingLevels;
}



void recording_and_write::OutCodTTBB( const string _file )
{
	local_time st;
	list<DateSurfase_TTBB>::iterator k;
	for (k = data_TTBB.begin(); k != data_TTBB.end(); ++k)
	{
		//Ôîðìèðîêàíèå èìåíè ôàéëà
		string name = StrNameFile( st, k->time, st.wDay , _file );
		fstream inFile( name.c_str() , ios_base::app );
		if(!inFile)
		{
			perror("Error open file!");
			cin.get();
			exit(1);
		}
		list<TTBB_Database>::iterator j;
		inFile << '\n' << "SpecialPoints" << "\n\n";
		k->data_.sort();
		for ( j = k->data_.begin(); j != k->data_.end(); ++j )
		{
			OutFileListTTBB(j, inFile);
			inFile << '\n';
		}
		inFile.close();
	}
}

void  recording_and_write::OutFileListTTBB( list<TTBB_Database>::iterator j, fstream & inFile )
{
	list<Temp_Base>::iterator L;
	for ( L = j->level.begin(); L != j->level.end(); ++L )
	{
		inFile <<  "IND=" << j->number.district_number;
		if(j->number.station_number < 100) inFile<<"0";
		inFile<<j->number.station_number<<"  ";
		int pressure = L->pressure;
		inFile <<  "P=" ;
		if( pressure <= 99) pressure+=1000;//äàííûé ñëó÷àé ñðàáàòûâàåò òîëüêî òîãäà êîãäà
		if (pressure < 1000)inFile << " ";
		if (pressure < 100)inFile << " ";
		if (pressure < 10)inFile << " ";
		inFile << pressure << "  ";
		inFile <<  "T=" ;
		double temp = L->info_temp.temp;
		if (temp > 0 )inFile << " ";
		if (fabs(temp) < 10)inFile << " ";
		if (fabs(temp) == 0)inFile << " ";
		inFile << temp;
		if((temp - (int)temp) == 0) inFile << ".0";
		inFile << '\n' ;
	}
}

string recording_and_write::StrNameFile(local_time st, int time, int date, string _file )
{
	string name;
	stringstream time_name;
	time_name << _file << "/SpecialPoints" << date;
	if(st.wMonth < 10) time_name << "0";
	time_name << st.wMonth << st.wYear << "_" << time << ".txt";
	time_name >> name;
	return name;
}


bool recording_and_write::LeapYear( local_time st )
{
	bool Leap = false;
	int year = st.wYear;
	if (fmodl(year,4)==0)
	{
		if (fmodl(year,100)==0)
		{
			if (fmodl(year,400)==0) Leap=1;
			else Leap=0;
		}else Leap=1;
	}else Leap=0;
	return Leap;
}
