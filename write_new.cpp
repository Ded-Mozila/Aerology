#include "WriteFile.h"
#include <string>
#include <cmath>
#include "settings.h"

void recording_and_write::WriteFile(const string _file )
{

	OutCodTTAA(_file);  		// создание файла и запись кода TTAA

	//OutCodTTCC(_file);			// запись в созданный уже файла кода TTCC

	// OutCodTTBB(1);				// запись в созданный уже файла кода TTBB


	// //OutCodTTBB(2);			// запись в созданный уже файла кода TTDD

	// OutSpecialPointWind();

	finishApp(inFile_00);
	finishApp(inFile_12);
	inFile_00.close();
	inFile_12.close();
}

void recording_and_write::OutCodTTAA(const string _file )
{
	if(!time_00.empty())Write_file_TTAA( 0, _file, inFile_00 );
		else cout << "00" ;				//Запуск программы на 0
	if(!time_12.empty())Write_file_TTAA( 12, _file, inFile_12 );
		else cout << "12" ;				//Запуск программы на 12
}
void recording_and_write::TopHeaderFileAerology(fstream & file, const int period,const local_time& st)
{
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
	file << ")\nСтандартные уровни (гПа): ТЕМР-А(1000-100), ТЕМР-С(70-10)\n"\
 		<<"Специальные точки по температуре: ТЕМР-B(1000-100 гПа), ТЕМР-D(выше 100 гПа) \n"\
 		<<"Специальные точки по ветру (гПа): ТЕМР-B(1000-100 гПа), ТЕМР-D(выше 100 гПа)\nрезкое изменение направления (≥10°) или скорости (≥5 м/сек)\n"\
 		<<"-------------------------------------------------------\nНЕТ ДАННЫХ от станций\n";
}
void recording_and_write::Write_file_TTAA(const int period , const string _file, fstream & file  )
{
	local_time st;										// Получение системного времени
	string name = StrNameFile( st, period , _file );	//Создание имени файла для вывода
	cout << name << endl;
	file.open( name.c_str() , ios_base::out );			//Создание файла куда записываются данные 
	if(!file)
	{
		perror("error open file SpecialPoints! write TTAA ");
		exit(1);
	}
	TopHeaderFileAerology(file,period,st);					// Вывод Заголовичных данных
	
	list<Station>::iterator i;						
	list<Station>::iterator i_begin;					// Начальный итератор списка данных для выбора периода обрадотки
	list<Station>::iterator i_end;						// Завершающий итератор списка данных для выбора периода обрадотки
	// Выбор итераторов обработки даннных за 00 или 12 часов
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
	//Вывод станций с которых не пришла и информация
 	int district = 0;
	for ( i = i_begin; i != i_end; ++i )
	{
		if (((*i).info != true || (*i).TTAA.information == false) && (*i).number != 0)
		{
			if(((*i).number)/1000 != district )// Сортировак по району нахождения станции
			{
				file << '\n';
				district = ((*i).number)/1000;
			}
			file << (*i).number << " ";
		}
	}
	//Выввод данных с уровна Земли
	file << "\n-------------------------------------------------------\nУровень земли:\n\n";
	for ( i = i_begin; i != i_end; ++i )
	{
		if ((*i).info == true)
		{
			TTAA_Database outA = (*i).TTAA;
			surface land = outA.land_surface;		//Поверхность земли 99
			file << (*i).number << " ";				//Ошибка с непоказанием найдена вывод был исправлен
			if ( outA.information == true && land.pressure != -1)
			{
				file << "Pzem=";
				if ((land.pressure) < 1000) file << " ";
				file << land.pressure << " ";
				file << "Tzem=";
				double temp = land.info_temp.temp;	//Температура
				if ( temp >= 0 ) file << " ";
				if (fabs(temp) < 10) file << " ";
				file << temp;
				if ((temp - (int)temp) == 0 && temp != 999) file << ".0";
				//Экспериментальный вывод времений!
				file << " ";
				if(outA.radioData.informationTime) OutTime(file,outA.radioData.GG,outA.radioData.gg);
				file << " ";
				if(outA.radioData.informationRadia) OutInfoZond(file,outA.radioData);
				file << '\n' ;
			}else file << "Нет данных\n";
			// if ( outA.information != true || land.pressure== -1) file << "Нет данных\n";
		}
	}
	file << "\n------------------------------------------------------\nСтандартные уровни (гПа): ТЕМР-А(1000-100), ТЕМР-С(70-10)\n\n";
	//Вывод стандартных уровней TTAA 
	for ( i = i_begin; i != i_end; ++i )
		if ((*i).info == true)
			WriteStandateSurfase((*i), file);		
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

void recording_and_write::WriteStandateSurfase(const Station time_station, fstream& file/*, bool StopProcesingLevels*/)
{
	TTAA_Database date = time_station.TTAA;
	list<standardSurface>::iterator i;

	if (date.information == true && time_station.info == true)
	{
		file << "TTAA " << date.code_ << "\n";
		int  StandartLevels[11] = {0,92,85,70,50,40,30,25,20,15,10};	
		bool emptyLevel = true;					// Пустой уровень
		for(i = date.level.begin(); i != date.level.end(); ++i )
		{
			standardSurface new_surfase = *i;
			double temp = new_surfase.data.info_temp.temp;				//Температура
			int number = new_surfase.height.number;						//Номер уровня 
			WIND wind = new_surfase.data.wind;
			// Проверка и запись отсутствующих уровней
			if(emptyLevel)
				for (int a = 0; a < 11; a++)
				{
					if (StandartLevels[a] == number)
					{
						emptyLevel = false;
						break;
					}
					else
					{
						file << "IND="<<  time_station.number <<" ";
						file << "P=";
						if (StandartLevels[a] != 0) file << " ";
						else file << "10";
						file << StandartLevels[a];
						if ( StandartLevels[a] == 92 ) file << "5";
						else file << "0";
						file << " ===================================\n";
					}
				}
			//Номер станции
			file << "IND="<<  time_station.number <<" ";
			OutPressure(file,10*number);								//Давление
			OutGeopotencial((new_surfase.height).height, number,file);	//Геопотенциал
			OutTemp(file,temp);											//Температура		
			OutWindDirection(file,wind.wind_direction);					//Направление ветра
			OutWindSpeed(file,wind.wind_speed);							//Скорость ветра
			OutDewpoint(file,new_surfase.data.info_temp.dewpoint);		//Дифицит точки росы
			file << "\n" ;
		}
		// Выввод данных кода С
		// if (time_station.info == true && time_station.TTCC.information == true)
		// {	
		// 	WriteStandateSurfase_TTCC( time_station.TTCC , file );
		// }
		if (!date.tropopause.empty())
		{
			file<< "Тропопауза:\n";
			list<surface>::iterator k;
			for (k = date.tropopause.begin(); k != date.tropopause.end() ; ++k)
			{
				surface Tr = (*k);
				file << "IND="<<  time_station.number <<" ";
				file << "P=";
				if ((Tr.pressure) < 1000) file << " ";
				file << Tr.pressure;										//Давление
				OutTemp(file,Tr.info_temp.temp);							//Температура		
				OutWindDirection(file,Tr.wind.wind_direction);				//Направление ветра
				OutWindSpeed(file,Tr.wind.wind_speed);						//Скорость ветра
				OutDewpoint(file,Tr.info_temp.dewpoint);					//Дифицит точки росы
				file << "\n" ;	

			}
		}
		if (!date.max_wind.empty())
		{
			file<< "Сведения о максимальном ветре :\n";
			list<surfaceWind>::iterator k;
			for (k = date.max_wind.begin(); k != date.max_wind.end() ; ++k)
			{
				surfaceWind Wn = (*k);
				file << "IND="<<  time_station.number <<" ";
				file <<"N="<< Wn.point\
				<< " P="<<Wn.data.pressure<< " ";
				OutWindDirection(file,Wn.data.wind.wind_direction);				//Направление ветра
				OutWindSpeed(file,Wn.data.wind.wind_speed);						//Скорость ветра
				if(Wn.shift.information == true)
				{
					file << " Vb=" << Wn.shift.up_speed\
					 << " Va=" << Wn.shift.below_speed;
				}
				file << "\n" ;	

			}
		}
			file << "\n" ;
	}
}

void recording_and_write::WriteStandateSurfase_TTCC( const TTCC_Database time_data, fstream & file)
{
	TTCC_Database date = time_data;
	list<standardSurface>::iterator i;
	if (date.information == true)
	{
		int  StandartLevels[6]= {70,50,30,20,10};
		bool emptyLevel = true;					// Пустой уровень
		for(i = date.level.begin(); i != date.level.end(); ++i )
		{
			standardSurface new_surfase = *i;
			double temp = new_surfase.data.info_temp.temp;				//Температура
			int number = new_surfase.height.number;						//Номер уровня
			WIND wind = new_surfase.data.wind;
			if (emptyLevel)
			{
				for (int a = 0; a < 5; a++)
				{
					if (StandartLevels[a] == new_surfase.height.number)
					{
						emptyLevel = false;
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
			//Номер станции
			file << "IND=" <<  time_data.number <<" ";
			OutPressure(file,number);										//Давление
			OutGeopotencial((new_surfase.height).height, number/10,file);	//Геопотенциал
			OutTemp(file,temp);												//Температура		
			OutWindDirection(file,wind.wind_direction);						//Направление ветра
			OutWindSpeed(file,wind.wind_speed);								//Скорость ветра
			OutDewpoint(file,new_surfase.data.info_temp.dewpoint);			//Дифицит точки росы
			file << "\n" ;
		}
	}
}
void recording_and_write::OutTime(fstream& file,const int& hh,const int& mm)
{
	file << "t=" << setfill (' ') <<  setw (3);
	file << hh << ":" <<  setw (2) << mm;
}

void recording_and_write::OutInfoZond(fstream& file,const InfoRadiationAmendment& info)
{
	file << "s=" << setfill (' ') <<  setw (3);
	file << info.s << ": rr=" <<  setw (2) << info.rr << " ss=" <<  setw (2) << info.ss;
}
void recording_and_write::OutPressure(fstream& file,int pressure)
{
	file << "P=" << setfill (' ') <<  setw (4);
	if ( pressure == 920 ) file << 925;
	else if (pressure == 0 ) file << 1000;
	else file << pressure;
}
void recording_and_write::OutGeopotencial(int geopot, int number, fstream& file)
{
	file << " H=";
	if (number == 0 || number >= 70 )	
		file << " "<< setfill ('0')<< setw (3) << geopot /*<< " "*/;
	if (number != 0 && number <= 50)
		file << setfill ('0')<< setw (3) << geopot << "0";
}
void recording_and_write::OutTemp(fstream& file, float temp)
{
	file << " T=";
	if ( temp == 999 ) file << " ";
	if ( temp >= 0 ) file << " ";
	if ( fabs(temp) < 10 ) file << " ";
	file << temp;
	if ( ( temp - (int)temp ) == 0 && temp != 999 ) file << ".0";
}
void recording_and_write::OutWindDirection(fstream& file, const int& speed)
{
	file << " d=";
	file << setfill (' ') <<  setw (3) << speed;
}
void recording_and_write::OutWindSpeed(fstream& file, const int& speed)
{
	file << " f=";
	file << setfill (' ') <<  setw (3) << speed;
}
void recording_and_write::OutDewpoint(fstream& file, const float& dewpoint)
{
	file << " D=";
	if ( abs(dewpoint) < 100 )file << " ";
	if ( abs(dewpoint) < 10 )file << " ";
	if (dewpoint == 999) file << " ";
	file << dewpoint;
	if ( ( dewpoint  - (int)dewpoint ) == 0 && dewpoint != 999 ) file << ".0";
	if ( dewpoint != 999 ) file << "0";
}