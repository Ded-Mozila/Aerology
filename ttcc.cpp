#include "WriteFile.h"
#include <vector>
void recording_and_write::TTCC( char * code , string strCode )
{
	try
	{
		TTCC_Database base;
		base.code_ = strCode;          
		//base.information = true;
		//cout << strCode << endl;
		bool theEnd = false;                    // Завершение программы
		int step = 1;                           // Шаг выполнения программы 
		bool wind_node = false;
		vector<string> VectorCode(stringToVectorString(code));
		//1 Нахождение времени и даты запуска зонда
		base.memory = DateTime(VectorCode[0],wind_node);
		//2 Находжение большого района и станции
		base.number = DistrictStation(VectorCode[1]);
		//3 Сортировка и находние поверхностей
		if( strstr( VectorCode.at(2).c_str(), "N" ) == NULL )
		{ 
			base.information = true;
			int i = 2;
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
				if(VectorCode.at(i).substr(0,2) == "77" || time_data.height.number == 66 ||time_data.height.number == 88 /*|| time_data.height.number/10 == 4*/)
				{
					//Условие выхода для тропопаузы
					noStLevel = true;
					break;
				}
				else
				{
					//Поиск уровня которого нет
					if(time_data.height.number/10 > base.memory.id_surface )
					{
						i+=1;
						time_data.data.info_temp = TempDewpoint(VectorCode.at(i));
						i+=1;
						time_data.data.wind = Wind(VectorCode.at(i),time_data.data.wind_node);
						i+=1; 
					}
					else
					{
						i+=1;
						time_data.data.info_temp = TempDewpoint(VectorCode.at(i));
						i+=1;
						if(VectorCode.at(i).substr(0,2) == "77" || VectorCode.at(i).substr(0,2) == "66" ||VectorCode.at(i).substr(0,2) == "88" /*|| time_data.height.number/10 == 4*/)
						{
							//Условие выхода для тропопаузы
							cout << "UPS2\n"\
							<< strCode << "\n" << VectorCode.at(i) << endl;
							noStLevel = true;
						}
						else
						{
							time_data.data.wind = Wind(VectorCode.at(i),time_data.data.wind_node);
							i+=1;
						}
					}
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
						surface trop;
						if(heightTrop.height == 999) {break;}
						else
						{
							trop.information = true;
							trop.wind_node = wind_node;
							trop.pressure = metround(heightTrop.height/10.0);
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
							wind.data.pressure = metround(heightTrop.height/10.0);
							wind.data.wind = Wind(VectorCode.at(i),wind_node);
							i += 1;//Преход к следующей группе
							if(i >= VectorCode.size()) {noStLevel = true;}
							else
							{							
								int vb = atoi(VectorCode.at(i).c_str());
								cout << " vv " << VectorCode.at(i).at(0) << "\n";
								if(VectorCode.at(i).at(0) == '4' /*&& ((vb/1000)%10 >= 0*/  && vb != 31313 )
								{
									wind.shift.information = true;
									wind.shift.up_speed = atoi((VectorCode.at(i).substr(1,2).c_str() != "//")? VectorCode.at(i).substr(1,2).c_str()  :"999");
									wind.shift.below_speed = atoi((VectorCode.at(i).substr(3,2).c_str() != "//")? VectorCode.at(i).substr(3,2).c_str()  :"999");
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
						cout << heightTrop.height <<" " << VectorCode.at(i-2) << "\n";
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
						noStLevel = true;
						break;
					}
				}
			}
		}
		else
		{
			base.information = false;
			theEnd = true;
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
				//cout << base.number << endl;
				(*i).info = true;
				(*i).TTCC = base;
				break;
			}
		}
	}
	catch(...)
	{
		cerr << "Error TTCC " << strCode <<  endl;
		// Error::no_ofErrors++;
	}

}
