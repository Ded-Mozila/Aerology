#include "WriteFile.h"

void recording_and_write::TTCC( char * code )
{
	TTCC_Database base;
	base.code_ = code;
	code = deleteEndl(code);                //Замена '\n' на пробел
	base.information = true;
	bool theEnd = false;                    // Завершение программы
	int step = 1;                           // Шаг выполнения программы 
	while((*code) != '\0')
	{
		switch (step)
		{
			case 1 : // Нахождение времени и даты запуска зонда
				{
					code = DateTime(code,base.memory);
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
					if( strstr( code, "NIL" ) == NULL ) //Проверка на отсутствие данных
					{
						base.information = true;
						//code = Pressure(code, base.land_surface.pressure);  
						//code = ReturnSurface( code , base.land_surface );
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
						//1. Нахождение стандартного изобаричких поверхностей и её высоты
						code = NumberHeight( code, time_data.height, i);
						//2. ...
						if (i == STOP)
							break;
						code = ReturnSurface( code, time_data.data );
						//3. Добавление элемента в конец списока
						base.level.push_back(time_data);

					}
					theEnd = true;
					break;

				}
		default:
			break;
		}
		if(theEnd  == true)
			break;
	}
	// Заполение списка ТТCC данными
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
	for ( i =i_begin; i != i_end; ++i )
	{
		if(base.number == (*i).number )
		{
			(*i).info = true;
			(*i).TTCC = base;
			break;
		}
	}

}