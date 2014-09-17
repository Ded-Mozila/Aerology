// #include "WriteFile.h"

// void recording_and_write::TTCC( char * code )
// {
// 	code = deleteEndl(code);                //Замена '\n' на пробел
// 	DateSurfase_TTCC new_base_;
// 	TTCC_Database base;
// 	base.information = true;
// 	bool theEnd = false;                    // Завершение программы
// 	int step = 1;                           // Шаг выполнения программы 
// 	while((*code) != '\0')
// 	{
// 		switch (step)
// 		{
// 			case 1 : // Нахождение времени и даты запуска зонда
// 				{
// 					code = DateTime(code,base.memory);
// 					step++; // преход к новому шагу
// 					break;
// 				}
// 			case 2 : // Находжение большого района и станции
// 				{
// 					code = DistrictStation(code, base.number);
// 					step++; // преход к новому шагу
// 					break;
// 				}
// 			case 3 : // Сортировка и находние поверхностей
// 				{
// 					if( strstr( code, "NIL" ) == NULL ) //Проверка на отсутствие данных
// 					{
// 						base.information = true;
// 						//code = Pressure(code, base.land_surface.pressure);  
// 						//code = ReturnSurface( code , base.land_surface );
// 					}
// 					else
// 					{
// 						theEnd = true;
// 						base.information = false;
// 					}
// 					step++; // преход к новому шагу
// 					break;
// 				}
// 			case 4 :
// 				{
// 					int i = 1;
// 					while((*(code)) != '\0' )
// 					{

// 						standardSurface time_data;
// 						//1. Нахождение стандартного изобаричких поверхностей и её высоты
// 						code = NumberHeight( code, time_data.height, i);
// 						//2. ...
// 						if (i == STOP)
// 							break;
// 						code = ReturnSurface( code, time_data.data );
// 						//3. Добавление элемента в конец списока
// 						base.level.push_back(time_data);

// 					}
// 					theEnd = true;
// 					break;

// 				}
// 		default:
// 			break;
// 		}
// 		if(theEnd  == true)
// 			break;
// 	}
// 	// Заполение списка ТТАА данными
// 	if(data_TTCC.size() == 0)
// 	{
// 		TimePeriod( base.memory.date, base.memory.time, new_base_.date, new_base_.time);
// 		new_base_.data_.push_back( base );
// 		data_TTCC.push_back( new_base_ );
// 	}
// 	else
// 	{
// 		bool check = false;
// 		list<DateSurfase_TTCC>::iterator i;
// 		for ( i = data_TTCC.begin(); i != data_TTCC.end(); ++i )
// 		{
// 			//Упростить 
// 			if( ((i->date == ( base.memory.date+1 ) && base.memory.time == 23  ) /* Если даты совпадают и время 23*/\
// 				|| \
// 				( i->date == ( base.memory.date ) && \
// 				( base.memory.time == 0  || base.memory.time == 1 ) )) && \
// 				( i->time  == 0)) //
// 			{
// 				check = true;
// 				i->data_.push_back( base );
// 				break;
// 			}
// 			if( i->date == base.memory.date  && ( base.memory.time == 11  || base.memory.time == 12  || base.memory.time == 13) &&  i->time == 12 ) //
// 			{
// 				check = true;
// 				i->data_.push_back( base );
// 				break;
// 			}
// 		}
// 		if(check == false)
// 		{
// 			TimePeriod( base.memory.date, base.memory.time, new_base_.date, new_base_.time);
// 			new_base_.data_.push_back( base );
// 			data_TTCC.push_back( new_base_ );
// 		}
// 	}

// }