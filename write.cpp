#include "WriteFile.h"
#include <string>
#include <cmath>
#include "settings.h"

void recording_and_write::WriteFile(const string _file )
{

	OutCodTTAA(_file);  // создание файла и запись кода TTAA

	OutCodTTBB(1);	// запись в созданный уже файла кода TTBB

	OutCodTTCC(_file);	// запись в созданный уже файла кода TTCC

	OutCodTTBB(2);	// запись в созданный уже файла кода TTDD

	OutSpecialPointWind();

	finishApp(inFile_00);
	finishApp(inFile_12);
	inFile_00.close();
	inFile_12.close();
}

void recording_and_write::finishApp(fstream & file )
{
	local_time st;
	file << "------------------------------------------------\n\nЗавершение программы \"отбор аэрологии\" " \
	<< setfill ('0')\
	<< setw (2)	<< st.whour << ":" << setw (2) << st.wmin << ":" << setw (2) << st.wsec << "  "\
	<< setw (2) << setw (2) << st.wDay <<"."<< setw (2)  << st.wMonth << "." << setw (4) << st.wYear << " ВСВ ";
}

void recording_and_write::Write_file_TTAA(int period , const string _file, fstream & file  )
{
	local_time st;
	string name = StrNameFile( st, period , _file );

	file.open( name.c_str() , ios_base::out );
	if(!file)
	{
		perror("error open file SpecialPoints! write TTAA ");
		exit(1);
	}
	file << "Старт программы отбора аэрологии: " << setfill ('0') << setw (2) << st.whour \
		<< ":"<< setw (2)  << st.wmin << ":" << setw (2) << st.wsec << " ВСВ "\
		<< setw (2) << st.wDay <<"."<< setw (2)  << st.wMonth << "." << setw (4) << st.wYear\
		<< "\n-------------------------------------------------------\nОтбор аэрологии из базы данных ТПК \"Прометей\" за срок "\
		<< setw (2) << period << " ВСВ " << setw (2) << st.wDay <<"."<< setw (2)  << st.wMonth << "." << setw (4) << st.wYear \
		<< "\n(Выборка за период: ";
	switch (period) //12 или 00
		{
		case 0:// 00 время это промежуток времени от 23:00 - 01:00
			{
				ViewTimePeriod_00_(file);
				break;
			}
		case 12:// 12 время этого промежутка от 11:00 до 13:00
			{
				ViewTimePeriod_12_(file);
				break;
			}
		default:
			{
				cout << "Fail";
				break;
			}
		}
	file << ")\n-------------------------------------------------------\nНЕТ ДАННЫХ от станций\n";
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
		if (((*i).info != true || (*i).TTAA.information == false) && (*i).number != 0)
		{
			file << (*i).number << " ";
			k++;
		}
		if(k == 5)
		{
			k = 0;
			file << "\n";
		}
	}
	file << "\n-------------------------------------------------------\nУровень земли:\n\n";
	for ( i = i_begin; i != i_end; ++i )
	{
		if ((*i).info == true)
		{
			TTAA_Database outA = (*i).TTAA;
			surface DAT = outA.land_surface;
			double temp = DAT.info_temp.temp;
			file << outA.number << " ";
			if ( outA.information == true )
			{
				file << "Pzem=";
				if ((DAT.pressure) < 1000) file << " ";
				file << DAT.pressure << " ";
				file << "Tzem=";
				if ( temp >= 0 ) file << " ";
				if (fabs(temp) < 10) file << " ";
				file << temp;
				if ((temp - (int)temp) == 0 && temp != 999) file << ".0";
				file << '\n' ;
			}
			if ( outA.information != true )
			{
				file << "Нет данных\n";
			}
		}
		
	}
	file << "\n------------------------------------------------------\nStandartLevels\n\n";

	for ( i = i_begin; i != i_end; ++i )
	{	
		bool controllerEmptyLevels = true;
		if ((*i).info == true)
		{
			controllerEmptyLevels = WriteStandateSurfase( (*i).TTAA , file ,controllerEmptyLevels);
		}
		
	}
}

void recording_and_write::OutCodTTAA( const string _file )
{
	if(time_00.size() != 0)Write_file_TTAA( 0, _file, inFile_00 );
		else cout << "00" ;				//Запуск программы на 0
	if(time_12.size() != 0)Write_file_TTAA( 12, _file, inFile_12 );
		else cout << "12" ;			//Запуск программы на 0
}


void recording_and_write::ViewTimePeriod_00_(fstream & file)
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
		file << "0";
	file << old_day << ".";
	if (old_month < 10)
		file << "0";
	file << old_month << "." << old_year  << " 23:00 - ";
	// Начало измерений
	if (day < 10) 
		file << "0";
	file << day << ".";
	if (month < 10)
		file << "0";
	file << month << "." << year  << " 01:00";

}

void recording_and_write::ViewTimePeriod_12_(fstream & file)
{
	long day = data%100;
	long month = ((data/100)%100);
	long year = data/10000;
	for (int h=0 ; h <= 1; h++)
	{
		if(day < 10) 
			file << "0";
		file << day << ".";
		if(month < 10) 
			file << "0";
		file << month << "." << year;
		if (h == 0) file << " 10:00 - ";
			else file << " 12:00";
	}
}

void recording_and_write::WriteLand( const TTAA_Database time_data,fstream & file )
{
	surface DAT = time_data.land_surface;
	double temp = DAT.info_temp.temp;
	file <<  time_data.number <<" " ;
	if ( DAT.information == true )
	{
		file << "Pzem=";
		if ((DAT.pressure) < 1000) file << " ";
		file << DAT.pressure << " ";
		file << "Tzem=";
		if ( temp >= 0 )
		{
			file << " ";
		}
		if (fabs(temp) < 10) file << " ";
		file << temp;
		if ((temp - (int)temp) == 0 && temp != 999) file << ".0";
		file << '\n' ;
	}
	else
	{
		file << "Нет данных\n";
	}
}

bool recording_and_write::WriteStandateSurfase( const TTAA_Database time_data, fstream & file , bool StopProcesingLevels)
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
							file << "IND="<<  time_data.number <<" ";
							file << "P=";
							if (StandartLevels[a] != 0) file << " ";
							else file << "10";
							file << StandartLevels[a];
							if ( StandartLevels[a] == 92 ) file << "5";
							else file << "0";
							file << " ============================\n";
						}
					}
				}
				//Íîìåð ñòàíöèè è ðàéîíà
				file << "IND="<<  time_data.number <<" ";

				//äàâëåíèå
				file << "P=";
				if (number != 0) file << " ";
				else file << "10";
				file << number;
				if ( number == 92 ) file << "5";
				else file << "0";

				//òåìïåðàòóðà
				file << " T=";
				if ( temp == 999 ) file << " ";
				if ( temp >= 0 ) file << " ";
				if ( fabs(temp) < 10 ) file << " ";
				file << temp;
				if ( ( temp - (int)temp ) == 0 && temp != 999 ) file << ".0";

				//íàïðàâëåíèå âåòðà
				file << " d=";
				if ( wind.wind_direction < 100 ) file << " ";
				if ( wind.wind_direction < 10 ) file << " ";
				file << wind.wind_direction;

				//ñêîðîòü âåòðà
				file << " f=";
				if ( abs(wind.wind_speed) < 100 )file << " ";
				if ( abs(wind.wind_speed) < 10 )file << " ";
				file << wind.wind_speed;

				//äåôèöèò òî÷êè ðîñû
				file << " D=";
				double dewpoint = new_surfase.data.info_temp.dewpoint;
				if ( abs(dewpoint) < 100 )file << " ";
				if ( abs(dewpoint) < 10 )file << " ";
				file << dewpoint;
				if ( ( dewpoint  - (int)dewpoint ) == 0 && dewpoint != 999 ) file << ".0";
				if ( dewpoint != 999 ) file << "0";
				file << "\n" ;
		}
		file << "\n" ;
	}
	return StopProcesingLevels;
}



void recording_and_write::OutCodTTBB( int key )
{
	//Вывод особых точек по температуре
	if(time_00.size() != 0)Write_file_TTBB( 0 ,inFile_00 , key );
		else cout << "00" ;				//Запуск программы на 0
	if(time_12.size() != 0)Write_file_TTBB( 12, inFile_12 , key );
		else cout << "12" ;			//Запуск программы на 0
}
void recording_and_write::Write_file_TTBB( int period , fstream & file , int key)
{
	TTBB_Database time_data;
	file << "-------------------------------------------------------\nSpecialPoints ";
	if( key == 1)
		file << "TTBB";
	else
		file << "TTDD";
	file << " Temp\n\n";
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
	for ( i = i_begin; i != i_end; ++i )
	{
		if( key == 1 ) time_data = (*i).TTBB;
		else time_data = (*i).TTDD;
		if ( time_data.information == true )
		{
			OutFileListTTBB(time_data, file ,key);
			if( time_data.level.size() != 0 )file << '\n';
		}
	}
}

void  recording_and_write::OutFileListTTBB(  TTBB_Database j, fstream & file, int key)
{
	list<Temp_Base>::iterator L;
	for ( L = j.level.begin(); L != j.level.end(); ++L )
	{
		if(L->information == true)
		{
			file <<  "IND=" << j.number;
			int pressure = L->pressure;
			file <<  " P=" ;
			if( pressure <= 99 && key == 1) pressure+=1000;//äàííûé ñëó÷àé ñðàáàòûâàåò òîëüêî òîãäà êîãäà
			file << setfill (' ') << setw (4) << pressure << " ";
			file <<  "T=" ;
			double temp = L->info_temp.temp;
			if (temp > 0 )file << " ";
			if (fabs(temp) < 10)file << " ";
			if (fabs(temp) == 0)file << " ";
			file << temp;
			if((temp - (int)temp) == 0 && temp != 999) file << ".0";
			file << '\n' ;
		}
	}
}

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

void recording_and_write::OutCodTTCC( const string _file )
{
	if(time_00.size() != 0)Write_file_TTCC( 0, _file, inFile_00);
		else cout << "00" ;				//Запуск программы на 0
	if(time_12.size() != 0)Write_file_TTCC( 12, _file, inFile_12 );
		else cout << "12" ;			//Запуск программы на 0
}
void recording_and_write::Write_file_TTCC( int period, const string _file, fstream & file )
{
	file << "-------------------------------------------------------\nSpecialPoints TTCC\n\n";
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
//	cout << "out\n";
	for ( i = i_begin; i != i_end; ++i )
		if ((*i).info == true && (*i).TTCC.information == true)
		{	
			//cout << (*i).TTCC.number << '\n';
			bool controllerEmptyLevels = true;
			controllerEmptyLevels = WriteStandateSurfase_TTCC( (*i).TTCC , file ,controllerEmptyLevels);
			if( (*i).TTCC.level.size() != 0 )file << '\n'  ;
		}
}
bool recording_and_write::WriteStandateSurfase_TTCC( const TTCC_Database time_data, fstream & file , bool StopProcesingLevels)
{
	TTCC_Database date = time_data;
	list<standardSurface>::iterator i;
	if (date.information == true)
	{

		for(i = date.level.begin(); i != date.level.end(); ++i )
		{
			int  StandartLevels[6] = {70,50,30,20,10};
			standardSurface new_surfase = *i;
			double temp = new_surfase.data.info_temp.temp;				//Òåìïåðàòóðà
			int number = new_surfase.height.number;						//Íîìåð óðîâíÿ
			WIND wind = new_surfase.data.wind;
			if (StopProcesingLevels)
			{
				for (int a = 0; a < 5; a++)
				{
					if (StandartLevels[a] == new_surfase.height.number)
					{
						StopProcesingLevels = false;
						break;
					}
					else
					{
						file << "IND="<<  time_data.number <<" ";
						file << "P=";
						if (StandartLevels[a] != 0) file << " ";
						else file << "10";
						file << StandartLevels[a];
						if ( StandartLevels[a] == 92 ) file << "5";
						else file << "0";
						file << "==========================\n";
					}
				}
			}
			//Íîìåð ñòàíöèè è ðàéîíà
			file << "IND="<<  time_data.number <<" ";

			//äàâëåíèå
			file << "P=";
			if (number != 0) file << " ";
			else file << "10";
			file << number;
			if ( number == 92 ) file << "5";

			//òåìïåðàòóðà
			file << " T=";
			if ( temp == 999 ) file << " ";
			if ( temp >= 0 ) file << " ";
			if ( fabs(temp) < 10 ) file << " ";
			file << temp;
			if ( ( temp - (int)temp ) == 0 && temp != 999 ) file << ".0";

			//íàïðàâëåíèå âåòðà
			file << " d=";
			if ( wind.wind_direction < 100 ) file << " ";
			if ( wind.wind_direction < 10 ) file << " ";
			file << wind.wind_direction;

			//ñêîðîòü âåòðà
			file << " f=";
			if ( abs(wind.wind_speed) < 100 )file << " ";
			if ( abs(wind.wind_speed) < 10 )file << " ";
			file << wind.wind_speed;

			//äåôèöèò òî÷êè ðîñû
			file << " D=";
			double dewpoint = new_surfase.data.info_temp.dewpoint;
			if ( abs(dewpoint) < 100 )file << " ";
			if ( abs(dewpoint) < 10 )file << " ";
			file << dewpoint;
			if ( ( dewpoint  - (int)dewpoint ) == 0 && dewpoint != 999 ) file << ".0";
			if ( dewpoint != 999 ) file << "0";
			file << "\n" ;
		}
	}
	return StopProcesingLevels;
}

void recording_and_write::OutSpecialPointWind(void)
{
	//BB
	if(time_00.size() != 0)Write_file_Wind( 0, inFile_00 , 1 );
		else cout << "00" ;				//Запуск программы на 0
	if(time_12.size() != 0)Write_file_Wind( 12, inFile_12 , 1 );
		else cout << "12" ;			//Запуск программы на 0
	//DD
	if(time_00.size() != 0)Write_file_Wind( 0, inFile_00 , 2 );
		else cout << "00" ;				//Запуск программы на 0
	if(time_12.size() != 0)Write_file_Wind( 12, inFile_12 , 2 );
		else cout << "12" ;			//Запуск программы на 0
}
void recording_and_write::Write_file_Wind( int period, fstream & file, int key)
{
	TTBB_Database time_data;
	file << "-------------------------------------------------------\nSpecialPoints ";
	if( key == 1)
		file << "TTBB";
	else
		file << "TTDD";
	file << " Wind\n\n";
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
	for ( i = i_begin; i != i_end; ++i )
	{
		if( key == 1 ) time_data = (*i).TTBB;
		else time_data = (*i).TTDD;

		if ((*i).info == true && time_data.information == true)
		{
			OutFileListWind(time_data, file ,key);
			if( time_data.level_wind.size() != 0 )file << '\n';
		}
	}
}
void recording_and_write::OutFileListWind(TTBB_Database time_data, fstream & file, int key)
{
	list<Wind_Base>::iterator L;
	for ( L = time_data.level_wind.begin(); L != time_data.level_wind.end(); ++L )
	{
		file <<  "IND=" << time_data.number;
		if(L->information ==false) file << " ==================\n";
		else
		{
			file <<  " P=" ;
			if( L->pressure <= 99 && key == 1) (*L).pressure+=1000;//äàííûé ñëó÷àé ñðàáàòûâàåò òîëüêî òîãäà êîãäà
			file << setfill (' ') << setw (4) << L->pressure\
			<< " d=" << setw (3) << (*L).wind.wind_direction\
			<< " f=" << setw (3) << (*L).wind.wind_speed << '\n' ;
		}
	}
}
