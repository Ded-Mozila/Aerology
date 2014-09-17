#include "WriteFile.h"
#include <string>
#include <cmath>
#include "settings.h"
#include <iomanip>
void recording_and_write::WriteFile(const string _file )
{

	OutCodTTAA(_file);  // создание файла и запись кода TTAA

	// OutCodTTBB(_file);	// запись в созданный уже файла кода TTBB

	// OutCodTTCC(_file);	// запись в созданный уже файла кода TTCC

}
void recording_and_write::Write_file_TTAA(int period , const string _file )
{
	local_time st;
	string name = StrNameFile( st, period , _file );
	inFile.open( name.c_str() , ios_base::out );
	if(!inFile)
	{
		perror("error open file SpecialPoints! ");
		exit(1);
	}
	//Вывод
	/*stringstream ss;
	ss << "Старт программы отбора аэрологии:";
	02:00:00 ВСВ 11.09.2014*/
	
	inFile << "Старт программы отбора аэрологии: " << setfill ('0') << setw (2) << st.whour \
		<< ":"<< setw (2)  << st.wmin << ":" << setw (2) << st.wsec << " ВСВ "\
		<< setw (2) << st.wDay <<"."<< setw (2)  << st.wMonth << "." << setw (2) << st.wYear\
		<< "\n-------------------------------------------------------\nОтбор аэрологии из базы данных ТПК \"Прометей\" за срок "\
		<< setw (2) << period << " ВСВ " << setw (2) << st.wDay <<"."<< setw (2)  << st.wMonth << "." << setw (2) << st.wYear \
		<< "\n(Выборка за период: ";
	switch (period) //12 или 00
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
	inFile << ")\n-------------------------------------------------------\n";
	inFile << "НЕТ ДАННЫХ от станций\n";





	list<Station>::iterator i;
	list<Station>::iterator i_begin;
	list<Station>::iterator i_end;
	if( period == 0 )			// 00
	{
		i_begin = time_00.begin();
		i_end = time_00.end();
	}
	if( period == 12 )		// 12
	{
		i_begin = time_12.begin();
		i_end = time_12.end();
	}
	int k = 0;
	for ( i = i_begin; i != i_end; ++i )
	{
		if ((*i).info == true)
		{
			if ( (*i).TTAA.information == false)
			{
				inFile << (*i).TTAA.number << " ";
				k++;
			}
		}
		if(k == 4)
		{
			k = 0;
			inFile << "\n";
		}
	}
	inFile << "\n-------------------------------------------------------\n";
	inFile << "Уровень земли:\n\n";
	for ( i = i_begin; i != i_end; ++i )
	{
		if ((*i).info == true)
		{
			TTAA_Database outA = (*i).TTAA;
			surface DAT = outA.land_surface;
			double temp = DAT.info_temp.temp;
			inFile << outA.number <<" " ;
			if ( outA.information == true )
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
			if ( outA.information != true )
			{
				inFile << "Нет данных\n";
			}
		}
		
	}
	inFile << '\n' <<  "StandartLevels" << "\n";

	for ( i = i_begin; i != i_end; ++i )
	{	
		bool controllerEmptyLevels = true;
		controllerEmptyLevels = WriteStandateSurfase( (*i).TTAA , inFile ,controllerEmptyLevels);
	}
	inFile.close();
}

void recording_and_write::OutCodTTAA( const string _file )
{
	if(time_00.size() != 0)Write_file_TTAA( 0, _file );
		else cout << "00" ;				//Запуск программы на 0
	if(time_12.size() != 0)Write_file_TTAA( 12, _file );
		else cout << "12" ;			//Запуск программы на 0
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
			if (LeapYear(st.wYear)) monline[1]=29;
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
		if (h == 0) inFile << " 10:00 - ";
			else inFile << " 12:00";
	}
}

void recording_and_write::WriteLand( const TTAA_Database time_data,fstream & inFile )
{
	surface DAT = time_data.land_surface;
	double temp = DAT.info_temp.temp;
	inFile <<  time_data.number <<" " ;
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
							inFile << "IND="<<  time_data.number <<" ";
							inFile << "P=";
							if (StandartLevels[a] != 0) inFile << " ";
							else inFile << "10";
							inFile << StandartLevels[a];
							if ( StandartLevels[a] == 92 ) inFile << "5";
							else inFile << "0";
							inFile << "==========================\n";
						}
					}
				}
				//Íîìåð ñòàíöèè è ðàéîíà
				inFile << "IND="<<  time_data.number <<" ";

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
		}
		inFile << "\n" ;
	}
	return StopProcesingLevels;
}



// void recording_and_write::OutCodTTBB( const string _file )
// {
// 	list<DateSurfase_TTBB>::iterator k = data_TTBB.begin();
// 	//Тут закоментирована версия программы которая должна анализировать несколько файлов
// 	//for (k = data_TTBB.begin(); k != data_TTBB.end(); ++k)
// 	//{
// 		list<TTBB_Database>::iterator j;
// 		InFile << '\n' << "SpecialPoints" << "\n\n";
// 		k->data_.sort();
// 		for ( j = k->data_.begin(); j != k->data_.end(); ++j )
// 		{
// 			OutFileListTTBB(j, InFile);
// 			if (j->information)
// 				InFile << '\n';
// 		}
		
// 	//}
// }

// void  recording_and_write::OutFileListTTBB( list<TTBB_Database>::iterator j, fstream & writeFileTTBB )
// {
// 	list<Temp_Base>::iterator L;
// 	for ( L = j->level.begin(); L != j->level.end(); ++L )
// 	{
// 		writeFileTTBB <<  "IND=" << j->number;
// 		int pressure = L->pressure;
// 		writeFileTTBB <<  "P=" ;
// 		if( pressure <= 99) pressure+=1000;//äàííûé ñëó÷àé ñðàáàòûâàåò òîëüêî òîãäà êîãäà
// 		if (pressure < 1000)writeFileTTBB << " ";
// 		if (pressure < 100)writeFileTTBB << " ";
// 		if (pressure < 10)writeFileTTBB << " ";
// 		writeFileTTBB << pressure << "  ";
// 		writeFileTTBB <<  "T=" ;
// 		double temp = L->info_temp.temp;
// 		if (temp > 0 )writeFileTTBB << " ";
// 		if (fabs(temp) < 10)writeFileTTBB << " ";
// 		if (fabs(temp) == 0)writeFileTTBB << " ";
// 		writeFileTTBB << temp;
// 		if((temp - (int)temp) == 0) writeFileTTBB << ".0";
// 		writeFileTTBB << '\n' ;
// 	}
// }

string recording_and_write::StrNameFile(local_time st, int time_, string _file )
{
	string name;
	stringstream time_name;
	time_name << _file << "/SpecialPoints" ;
	if(st.wDay < 10) 
		time_name << "0";
	 time_name << st.wDay;
	if(st.wMonth < 10) 
		time_name << "0";
	time_name << st.wMonth << st.wYear << "_";
	if (time_ == 0)
		time_name << "0";
	time_name << time_ << ".txt";
	time_name >> name;
	return name;
}


// void recording_and_write::OutCodTTCC( const string _file )
// {
// 	local_time st;
// 	list<DateSurfase_TTCC>::iterator i = data_TTCC.begin();
// 	i->data_.sort();
// 	InFile << "\n\n" << "SpecialPoints_TTCC";
// 	list<TTCC_Database>::iterator J;
// 	InFile << '\n' <<  "StandartLevels" << "\n";
// 	for ( J = i->data_.begin(); J != i->data_.end(); ++J )
// 	{	
// 		bool controllerEmptyLevels = true;
// 		controllerEmptyLevels = WriteStandateSurfase_TTCC( *J , InFile ,controllerEmptyLevels);
// 	}
// }
// bool recording_and_write::WriteStandateSurfase_TTCC( const TTCC_Database time_data, fstream & inFile , bool StopProcesingLevels)
// {
// 	TTCC_Database date = time_data;
// 	list<standardSurface>::iterator i;
// 	if (date.information == true)
// 	{

// 		for(i = date.level.begin(); i != date.level.end(); ++i )
// 		{
// 			int  StandartLevels[6] = {70,50,30,20,10};
// 			standardSurface new_surfase = *i;
// 			double temp = new_surfase.data.info_temp.temp;				//Òåìïåðàòóðà
// 			int number = new_surfase.height.number;						//Íîìåð óðîâíÿ
// 			WIND wind = new_surfase.data.wind;
// 			if (StopProcesingLevels)
// 			{
// 				for (int a = 0; a < 5; a++)
// 				{
// 					if (StandartLevels[a] == new_surfase.height.number)
// 					{
// 						StopProcesingLevels = false;
// 						break;
// 					}
// 					else
// 					{
// 						inFile << "IND="<<  time_data.number <<" ";
// 						inFile << "P=";
// 						if (StandartLevels[a] != 0) inFile << " ";
// 						else inFile << "10";
// 						inFile << StandartLevels[a];
// 						if ( StandartLevels[a] == 92 ) inFile << "5";
// 						else inFile << "0";
// 						inFile << "==========================\n";
// 					}
// 				}
// 			}
// 			//Íîìåð ñòàíöèè è ðàéîíà
// 			inFile << "IND="<<  time_data.number <<" ";

// 			//äàâëåíèå
// 			inFile << "P=";
// 			if (number != 0) inFile << " ";
// 			else inFile << "10";
// 			inFile << number;
// 			if ( number == 92 ) inFile << "5";
// 			else inFile << "0";

// 			//òåìïåðàòóðà
// 			inFile << " T=";
// 			if ( temp == 999 ) inFile << " ";
// 			if ( temp >= 0 ) inFile << " ";
// 			if ( fabs(temp) < 10 ) inFile << " ";
// 			inFile << temp;
// 			if ( ( temp - (int)temp ) == 0 && temp != 999 ) inFile << ".0";

// 			//íàïðàâëåíèå âåòðà
// 			inFile << " d=";
// 			if ( wind.wind_direction < 100 ) inFile << " ";
// 			if ( wind.wind_direction < 10 ) inFile << " ";
// 			inFile << wind.wind_direction;

// 			//ñêîðîòü âåòðà
// 			inFile << " f=";
// 			if ( abs(wind.wind_speed) < 100 )inFile << " ";
// 			if ( abs(wind.wind_speed) < 10 )inFile << " ";
// 			inFile << wind.wind_speed;

// 			//äåôèöèò òî÷êè ðîñû
// 			inFile << " D=";
// 			double dewpoint = new_surfase.data.info_temp.dewpoint;
// 			if ( abs(dewpoint) < 100 )inFile << " ";
// 			if ( abs(dewpoint) < 10 )inFile << " ";
// 			inFile << dewpoint;
// 			if ( ( dewpoint  - (int)dewpoint ) == 0 && dewpoint != 999 ) inFile << ".0";
// 			if ( dewpoint != 999 ) inFile << "0";
// 			inFile << "\n" ;
// 		}
// 		inFile << "\n" ;
// 	}
// 	return StopProcesingLevels;
// }
