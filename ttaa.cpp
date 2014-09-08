#include "WriteFile.h"

char* recording_and_write::deleteEndl( char* code )
{
	int len = strlen(code)+1;
	stringstream ss;
	for (int i = 0; i < len; ++i)
	{
		if (code[i] != '\r')
		{
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
	code = deleteEndl(code);				//Замена '\n' на пробел
	DateSurfase_TTAA new_base_;
	TTAA_Database base;
	base.information = true;
	bool theEnd = false;					// Завершение программы
	int step = 1;							// Шаг выполнения программы	
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
						code = Pressure(code, base.land_surface.pressure);  
						code = ReturnSurface( code , base.land_surface );
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
						//3. Добавление элемента в список
						base.level.push_back(time_data);

					}
					theEnd  = true;
					break;

				}
		default:
			break;
		}
		if(theEnd  == true)
			break;
	}
	// Заполение списка ТТАА данными
	if(data_TTAA.size() == 0)
	{
		TimePeriod( base.memory.date, base.memory.time, new_base_.date, new_base_.time);
		new_base_.data_.push_back( base );
		data_TTAA.push_back( new_base_ );
	}
	else
	{
		bool check = false;
		list<DateSurfase_TTAA>::iterator i;
		for ( i = data_TTAA.begin(); i != data_TTAA.end(); ++i )
		{
			//Упростить 
			if( ((i->date == ( base.memory.date+1 ) && base.memory.time == 23  ) /* Если даты совпадают и время 23*/\
				|| \
				( i->date == ( base.memory.date ) && \
				( base.memory.time == 0  || base.memory.time == 1 ) )) && \
				( i->time  == 0)) //
			{
				check = true;
				i->data_.push_back( base );
				break;
			}
			if( i->date == base.memory.date  && ( base.memory.time == 11  || base.memory.time == 12  || base.memory.time == 13) &&  i->time == 12 ) //
			{
				check = true;
				i->data_.push_back( base );
				break;
			}
		}
		if(check == false)
		{
			TimePeriod( base.memory.date, base.memory.time, new_base_.date, new_base_.time);
			new_base_.data_.push_back( base );
			data_TTAA.push_back( new_base_ );
		}
	}

}

//Íåâåðíûé ìåòîä â êàæäîì ìåñÿöå åñòü ýòà äàòà!!!!!!!
void recording_and_write::TimePeriod( const int newDate, const int NewTime, int& Date, int& Time ) // New* òå äàííûå êîòîðûå íóæíî çàïèñàòü â íîâûé áëîê äàííûõ Date and Time
{
	if( NewTime == 0 || NewTime == 1 || NewTime == 23 ) Time = 0;
	else Time = 12;
	Date = newDate;
	if ( NewTime == 23 )
	{
		int monline[4]={31,28,30,29};
		for (int i = 0; i < 4; i++)
		{
			if(Date == monline[i])
			{
				Date = 1;
				break;
			}
			else Date = newDate + 1;
		}
	}
}


int  recording_and_write::LastDayPrecedingMonth() /// Îïðåäåëåíèå ïîñëåäíåãî äíÿ ïðîøëîãî ìåñÿöà
{
	int oldMonth = 0;
	int oldDay = 0;
	local_time st; // ïîëó÷åíèå äàííûõ äàòû

	// Получение заначения высокосного года
	int monline[12]={31,28,31,30,31,30,31,31,30,31,30,31};
	if (LeapYear(st)) monline[1]=29;
	int month = st.wMonth;
	if(month == 0)
	{
		oldMonth = 11;
	}else oldMonth = month-1;
	oldDay = monline[oldMonth];
	return oldDay;

}
