#include "WriteFile.h"

char* recording_and_write::deleteEndl( char* code )
{
	//cout << code << endl;
	int len = strlen(code)+1;
	stringstream ss;
	for (int i = 0; i < len; ++i)
	{
		if (code[i] != '\r')
		{
			if(code[i] == '\n' && code[i+1] == '\0')
			{
				ss << '\0';
				break;
			}
			if(code[i] == '\n')
				ss << ' ';
			else ss << code[i];
		}	
	}
	string str(ss.str());
	strcpy(code,str.c_str());
	return code;
}
void recording_and_write::TTAA( char * code )
{
	TTAA_Database base;
	base.code_ = code;
	base.information = true;
	code = deleteEndl(code);code = deleteEndl(code);code = deleteEndl(code);code = deleteEndl(code);
	bool theEnd = false;					// Завершение программы
	int step = 1;							// Шаг выполнения программы
	bool wind_node = false;
	base.code_ = code;
	while((*code) != '\0')
	{
		switch (step)
		{
			case 1 : // Нахождение времени и даты запуска зонда
				{
					code = DateTime(code,wind_node,base.memory);
					step++; // преход к новому шагу
					break;
				}
			case 2 : // Находжение большого района и станции
				{
					code = DistrictStation(code, base.number);
					step++; // преход к новому шагу
					break;
				}
			case 3 : // Сортировка и находние поверхностей
				{

					if( strstr( code, "N" ) == NULL ) //Проверка на отсутствие данных
					{
						int i;
						base.information = true;
						code = Pressure(code, base.land_surface.pressure );  
						code = ReturnSurface( code , base.land_surface ,i );
					}
					else
					{	
						theEnd = true;
						base.information = false;
					}
					step++; // преход к новому шагу
					break;
				}
			case 4 :
				{
					int i = 1;
					while((*(code)) != '\0' )
					{
						standardSurface time_data;
						time_data.data.wind_node = wind_node;
						//1. Нахождение стандартного изобаричких поверхностей и её высоты
						surface trop;		// новая тропопауза
						surfaceWind wind77;	// Максимальный ветер
						WIND_SHIFT shift;	// Вертикальный сдвиг
						code = NumberHeight( code,trop,wind77,shift,time_data.height, i); //<<-- Анализ Тропопаузы))
						//2. ...
						if (i == 888 /*&& trop.information == true*/)
						{
							if(trop.information == true)base.tropopause.push_back(trop);
							else continue;
						}
						else
						if (((i == 777)||(i == 666))/*&& wind77.information == true*/)
						{
							if(wind77.information == true && wind77.data.pressure != -1)base.max_wind.push_back(wind77);
							else continue;
							
						}
						else 
						if (i == 444)
						{
							base.vertical.push_back(shift);
						}
						else
						if(i == 31313 )
						{
							theEnd  = true;
							break;
						}
						else
						{
							code = ReturnSurface( code, time_data.data, i);
							if (i == STOP)
							{
								base.tropopause.push_back(trop);// Провекка на пропущенные тропокаузы
								break;
							}
							//3. Добавление элемента в список
							base.level.push_back(time_data);
						}
					}
					// step++; // преход к новому шагу
					theEnd  = true;
					break;

				}
			case 5 :
				{
					break;
				}

			default:
				break;
		}
		if(theEnd  == true)
			break;
	}
	// Заполение списка ТТАА данными
	list<Station>::iterator i;
	list<Station>::iterator i_begin;
	list<Station>::iterator i_end;
	if( base.memory.time == 23 || base.memory.time == 0 || base.memory.time == 1 )			// 00
	{
		i_begin = time_00.begin();
		i_end = time_00.end();
	}
	if( base.memory.time == 11 || base.memory.time == 12 || base.memory.time == 13 )		// 12
	{
		i_begin = time_12.begin();
		i_end = time_12.end();
	}
	//bool qwerty = false;
	for ( i =i_begin; i != i_end; ++i )
	{
		if(base.number == (*i).number )
		{
			//qwerty = true;
			(*i).info = true;
			(*i).TTAA = base;		
			break;
		}
	}
	//if (qwerty == false ) cout << base.number << endl;
}
