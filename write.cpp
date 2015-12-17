#include "WriteFile.h"
#include "settings.h"

void recording_and_write::finishApp(fstream & file )
{
	local_time st;
	file << "------------------------------------------------\n\nЗавершение программы \"отбор аэрологии\" " \
	<< setfill ('0')\
	<< setw (2)	<< st.whour << ":" << setw (2) << st.wmin << ":" << setw (2) << st.wsec << "  "\
	<< setw (2) << setw (2) << st.wDay <<"."<< setw (2)  << st.wMonth << "." << setw (4) << st.wYear << " ВСВ ";
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

void recording_and_write::OutCodTTBB()
{
	//Вывод особых точек по температуре
	if(time_00.size() != 0)Write_file_TTBB( 0 ,inFile_00 );
		else cout << "00" ;				//Запуск программы на 0
	if(time_12.size() != 0)Write_file_TTBB( 12, inFile_12  );
		else cout << "12" ;			//Запуск программы на 0
}
void recording_and_write::Write_file_TTBB( int period , fstream & file )
{
	TTBB_Database time_data;
	file << "-------------------------------------------------------\nСпециальные точки по температуре: ТЕМР-B(1000-100 гПа), ТЕМР-D(выше 100 гПа) \n\n";
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
		time_data = (*i).TTBB;
		if ( time_data.information == true )
		{
			OutFileListTTBB(time_data, file ,1);
		}
		time_data = (*i).TTDD;
		if ( time_data.information == true )
		{
			OutFileListTTBB(time_data, file ,0);
		}
		if( (*i).TTBB.level.size() != 0 )file << '\n';
	}
}

void  recording_and_write::OutFileListTTBB(  TTBB_Database j, fstream & file, int key)
{
	list<Temp_Base>::iterator L;
	//file <<  j.code_ << endl;
	for ( L = j.level.begin(); L != j.level.end(); ++L )
	{
		if(L->information == true)
		{
			file <<  "IND=" << j.number;
			if(L->pressure == 999 && L->info_temp.temp == 999 && L->info_temp.dewpoint == 999)
				file << "===========================\n";
			else
			{
				int pressure = L->pressure;
				file <<  " P=" ;
				if( pressure <= 99 && key == 1) pressure+=1000;
				if(key != 1) { file << setfill (' ') << setw (4) << pressure/10 << "." << pressure%10 << ' ' ; }
				else {file << setfill (' ') << setw (4) << pressure << ".0 ";}
				OutTemp(file,L->info_temp.temp);
				OutDewpoint(file,L->info_temp.dewpoint);
				file << '\n' ;
			}
		}
	}
}


void recording_and_write::OutSpecialPointWind(void)
{
	if(time_00.size() != 0)Write_file_Wind( 0, inFile_00 , 1 );
		else cout << "00" ;				//Запуск программы на 0
	if(time_12.size() != 0)Write_file_Wind( 12, inFile_12 , 1 );
		else cout << "12" ;			//Запуск программы на 0
}
void recording_and_write::Write_file_Wind( int period, fstream & file, int key)
{
	TTBB_Database time_data;
	file << "-------------------------------------------------------\nСпециальные точки по ветру (гПа): ТЕМР-B(1000-100 гПа), ТЕМР-D(выше 100 гПа)\nрезкое изменение направления (≥10°) или скорости (≥5 м/сек)\n\n";
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
		time_data = (*i).TTBB;

		if ((*i).info == true && time_data.information == true)
		{
			OutFileListWind(time_data, file ,1);
		}
		time_data = (*i).TTDD;
		if ((*i).info == true && time_data.information == true)
		{
			OutFileListWind(time_data, file ,0);
		}

		if( (*i).TTBB.level_wind.size() != 0 )file << '\n';
	}
}
void recording_and_write::OutFileListWind(TTBB_Database time_data, fstream & file, int key)
{
	list<Wind_Base>::iterator L;
	//file << time_data.code_ << endl;
	for ( L = time_data.level_wind.begin(); L != time_data.level_wind.end(); ++L )
	{
		file <<  "IND=" << time_data.number;
		if(L->information ==false) file << " ==================\n";
		else
		{
			file <<  " P=" ;
			if( L->pressure <= 99 && key == 1) (*L).pressure+=1000;
			if(key != 1) { file << setfill (' ') << setw (4) << L->pressure/10 << "." << L->pressure%10 << ' ' ; }
			else {file << setfill (' ') << setw (4) << L->pressure << ".0 ";}
			OutWindDirection(file,(*L).wind.wind_direction);
			OutWindSpeed(file,(*L).wind.wind_speed);
			file << endl;
		}
	}
}
