#include "WriteFile.h"
#include <vector>
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

void recording_and_write::TTAA( char * code , string strCode)
{
	TTAA_Database base;
	base.code_ = code;
	base.information = true;
	//cout << strCode << endl;
	code = deleteEndl(code);
	bool theEnd = false;					// Завершение программы
	bool wind_node = false;
	base.code_ = strCode;
	istringstream arrayCode(code);
	string block;
	vector<string> VectorCode;
	while(arrayCode >> block)
	{
		VectorCode.push_back(block);
	}
	//1 Нахождение времени и даты запуска зонда
	base.memory = DateTime(VectorCode[0],wind_node);
	//2 Находжение большого района и станции
	base.number = DistrictStation(VectorCode[1]);
	//3 Сортировка и находние поверхностей
	if( strstr( VectorCode.at(2).c_str(), "N" ) == NULL )
	{ 
		base.information = true;
		base.land_surface.pressure = Pressure(VectorCode.at(2));
		base.land_surface.info_temp = TempDewpoint(VectorCode.at(3));
		base.land_surface.wind = Wind(VectorCode.at(4),base.land_surface.wind_node);
		//string verctorSur[11] = {"00","92","85","70","50","40","30","25","20","15","10"};
		int i = 5;
		bool noStLevel = false;
		//Цикл по Сандартным уровням
		while(i != VectorCode.size() || noStLevel == false)
		{

			standardSurface time_data;
			time_data.data.wind_node = wind_node; //В каких единицах ветер 
			//1. Нахождение стандартного изобаричких поверхностей и её высоты
			surface trop;		// Новая тропопауза
			surfaceWind wind77;	// Максимальный ветер
			WIND_SHIFT shift;	// Вертикальный сдвиг
			time_data.height = NumberHeight(VectorCode.at(i));
			//cout << VectorCode.at(i) << " ";
			
			if(time_data.height.number == 77 || time_data.height.number == 66 ||time_data.height.number == 88 /*|| time_data.height.number/10 == 4*/)
			{
				//Условие выхода для тропопаузы
				noStLevel = true;
				break;
			}
			else
			{
				i+=1;
				//cout << VectorCode.at(i) << " ";
				time_data.data.info_temp = TempDewpoint(VectorCode.at(i));
				i+=1;
				//cout << VectorCode.at(i) << "\n";
				time_data.data.wind = Wind(VectorCode.at(i),time_data.data.wind_node);
				i+=1; 
				//cout << time_data.height.number << ":" << time_data.height.height << ";" <<  time_data.data.info_temp.temp << ":"\
				//<< time_data.data.info_temp.dewpoint << ";" << time_data.data.wind.wind_speed << ":" << time_data.data.wind.wind_direction << "\n"; 				
				//3. Добавление элемента в список
				base.level.push_back(time_data);
			}


		}
		noStLevel = false;
		
		//Цикл по особым точкам
		while(noStLevel == false)
		{
			if(i >= VectorCode.size()) {noStLevel = true; break;}
			NUMBER_HEIGHT heightTrop = NumberHeight(VectorCode.at(i));
			i+=1; //Преход к следующей группе
			if(i >= VectorCode.size()) {noStLevel = true; break;}
			switch(heightTrop.number)
			{
				case 88 ://Цикл по тропапаузе code = 88
				{
					//cout << "88\n";
					surface trop;
					if(heightTrop.height == 999) {break;}
					else
					{
						trop.information = true;
						trop.wind_node = wind_node;
						trop.pressure = heightTrop.height;
						trop.info_temp = TempDewpoint(VectorCode.at(i));
						i+=1;//Преход к следующей группе
						if(i >= VectorCode.size()) {noStLevel = true; break;}// проверка на конец списка
						trop.wind = Wind(VectorCode.at(i),trop.wind_node);
						i+=1;//Преход к следующей группе
						if(i >= VectorCode.size()) {noStLevel = true; break;}// проверка на конец списка
						base.tropopause.push_back(trop);
					}

					break;
				}
				case 77 :
				case 66 : //Сведения о максимальном ветре 77 66 
				{
					surfaceWind wind;
					if(heightTrop.height == 999) { break;}
					else
					{
						wind.information = true;
						wind.data.wind_node = wind_node;
						wind.point = heightTrop.number;
						wind.data.pressure = heightTrop.height;
						wind.data.wind = Wind(VectorCode.at(i),wind_node);
						i += 1;//Преход к следующей группе
						//cout << base.number<<" we "<< heightTrop.number << ";"<< heightTrop.height<<"!" <<i <<"!" << VectorCode.size() <<"\n";
						if(i >= VectorCode.size()) {noStLevel = true;}
						else{
							
							int vb = atoi(VectorCode[i].c_str());
							if((vb/10000) == 4 && ((vb/1000)%10 >= 0  && vb != 31313 ))
							{
								wind.shift.information = true;
								wind.shift.up_speed = (vb/100)%100;
								wind.shift.below_speed = vb%100;
								i += 1;
							}
							else
							{
								wind.shift.information = false;
							}
						}
						base.max_wind.push_back(wind);
					}
					break;
				}
				case 31:
				{
					base.radioData.informationRadia = true;
					int srrss = atoi(VectorCode.at(i).c_str());
					base.radioData.s = srrss/10000;
					base.radioData.rr = (srrss/100)%100;
					base.radioData.ss = srrss%100;
					i +=1;
					///Время запуска запуска
					if(i >= VectorCode.size()) {noStLevel = true;}
					else
					{
						int XGGgg = atoi(VectorCode.at(i).c_str());
						if(XGGgg /10000 == 8)
						{
							base.radioData.informationTime = true;
							base.radioData.GG = (XGGgg /100)%100;
							base.radioData.gg = XGGgg%100;

						}
						else
						{
							cout << "Error " << VectorCode.at(i).c_str() << endl;
						}
					}
					theEnd = true;
					noStLevel = true;
					break;
				}
				default:
				{
					cout << "Error" << code << "\nBlock = " <<heightTrop.number<< " == "<< VectorCode.at(i-1) << "\n";
					theEnd = true;
					//snoStLevel = true;
					break;
				}
			}
		}
		//if(base.memory.id_surface == '3') cin >> i ;
	}
	else
	{
		base.information = false;
		theEnd = true;
	}
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
	//4 Стнадартные изобарические поверхности

	
	// {
	// 	standardSurface time_data;
	// 	time_data.data.wind_node = wind_node;
	// 	//1. Нахождение стандартного изобаричких поверхностей и её высоты
	// 	surface trop;		// новая тропопауза
	// 	surfaceWind wind77;	// Максимальный ветер
	// 	WIND_SHIFT shift;	// Вертикальный сдвиг
	// 	code = NumberHeight( code,trop,wind77,shift,time_data.height, i); //<<-- Анализ Тропопаузы))
	// 	//2. ...
	// 	if (i == 888 /*&& trop.information == true*/)
	// 	{
	// 		if(trop.information == true)base.tropopause.push_back(trop);
	// 		else continue;
	// 	}
	// 	else
	// 	if (((i == 777)||(i == 666))&& wind77.information == true)
	// 	{
	// 		if(wind77.information == true && wind77.data.pressure != -1)base.max_wind.push_back(wind77);
	// 		else continue;
			
	// 	}
	// 	else 
	// 	if (i == 444)
	// 	{
	// 		base.vertical.push_back(shift);
	// 	}
	// 	else
	// 	if(i == 31313 )
	// 	{
	// 		theEnd  = true;
	// 		break;
	// 	}
	// 	else
	// 	{
	// 		code = ReturnSurface( code, time_data.data, i);
	// 		if (i == STOP)
	// 		{
	// 			base.tropopause.push_back(trop);// Провекка на пропущенные тропокаузы
	// 			break;
	// 		}
	// 		//3. Добавление элемента в список
	// 		base.level.push_back(time_data);
	// 	}
	// }
	// // step++; // преход к новому шагу
	// theEnd  = true;
	// break;

	// while((*code) != '\0')
	// {
	// 	switch (step)
	// 	{
	// 		case 1 : // Нахождение времени и даты запуска зонда
	// 			{
	// 				code = DateTime(code,wind_node,base.memory);
	// 				step++; // преход к новому шагу
	// 				break;
	// 			}
	// 		case 2 : // Находжение большого района и станции
	// 			{
	// 				code = DistrictStation(code, base.number);
	// 				step++; // преход к новому шагу
	// 				break;
	// 			}
	// 		case 3 : // Сортировка и находние поверхностей
	// 			{

	// 				if( strstr( code, "N" ) == NULL ) //Проверка на отсутствие данных
	// 				{
	// 					int i;
	// 					base.information = true;
	// 					code = Pressure(code, base.land_surface.pressure );  
	// 					code = ReturnSurface( code , base.land_surface ,i );
	// 				}
	// 				else
	// 				{	
	// 					theEnd = true;
	// 					base.information = false;
	// 				}
	// 				step++; // преход к новому шагу
	// 				break;
	// 			}
	// 		case 4 :
	// 			{
	// 				int i = 1;
	// 				while((*(code)) != '\0' )
	// 				{
	// 					standardSurface time_data;
	// 					time_data.data.wind_node = wind_node;
	// 					//1. Нахождение стандартного изобаричких поверхностей и её высоты
	// 					surface trop;		// новая тропопауза
	// 					surfaceWind wind77;	// Максимальный ветер
	// 					WIND_SHIFT shift;	// Вертикальный сдвиг
	// 					code = NumberHeight( code,trop,wind77,shift,time_data.height, i); //<<-- Анализ Тропопаузы))
	// 					//2. ...
	// 					if (i == 888 /*&& trop.information == true*/)
	// 					{
	// 						if(trop.information == true)base.tropopause.push_back(trop);
	// 						else continue;
	// 					}
	// 					else
	// 					if (((i == 777)||(i == 666))/*&& wind77.information == true*/)
	// 					{
	// 						if(wind77.information == true && wind77.data.pressure != -1)base.max_wind.push_back(wind77);
	// 						else continue;
							
	// 					}
	// 					else 
	// 					if (i == 444)
	// 					{
	// 						base.vertical.push_back(shift);
	// 					}
	// 					else
	// 					if(i == 31313 )
	// 					{
	// 						theEnd  = true;
	// 						break;
	// 					}
	// 					else
	// 					{
	// 						code = ReturnSurface( code, time_data.data, i);
	// 						if (i == STOP)
	// 						{
	// 							base.tropopause.push_back(trop);// Провекка на пропущенные тропокаузы
	// 							break;
	// 						}
	// 						//3. Добавление элемента в список
	// 						base.level.push_back(time_data);
	// 					}
	// 				}
	// 				// step++; // преход к новому шагу
	// 				theEnd  = true;
	// 				break;

	// 			}
	// 		case 5 :
	// 			{
	// 				break;
	// 			}

	// 		default:
	// 			break;
	// 	}
	// 	if(theEnd  == true)
	// 		break;
	// }
	// Заполение списка ТТАА данными
	
