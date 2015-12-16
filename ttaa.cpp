#include "WriteFile.h"


void recording_and_write::TTAA( char * code , string strCode)
{
	try
	{
		//cout << strCode << endl;
		bool wind_node = false;
		vector<string> VectorCode(stringToVectorString(code));
		TTAA_Database base(strCode,DateTime(VectorCode[0],wind_node),DistrictStation(VectorCode[1]));
		//3 Сортировка и находние поверхностей
		if( strstr( VectorCode.at(2).c_str(), "N" ) == NULL )
		{ 
			base.information = true;
			if(2 < VectorCode.size())base.land_surface.pressure = Pressure(VectorCode.at(2));
			if(3 < VectorCode.size())base.land_surface.info_temp = TempDewpoint(VectorCode.at(3));
			if(4 < VectorCode.size())base.land_surface.wind = Wind(VectorCode.at(4),base.land_surface.wind_node);
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
				if(VectorCode.at(i).substr(0,2) == "77" || VectorCode.at(i).substr(0,2) == "66" ||VectorCode.at(i).substr(0,2) == "88" /*|| time_data.height.number/10 == 4*/)
				{
					//Условие выхода для тропопаузы
					noStLevel = true;
					break;
				}
				else
				{
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
			//cout << "i="<< i << endl;
			//Цикл по особым точкам
			while(noStLevel == false)
			{
				if(i >= VectorCode.size()) {noStLevel = true; break;}
				NUMBER_HEIGHT heightTrop = NumberHeight(VectorCode.at(i));
				i+=1; //Преход к следующей группе
				if(i >= VectorCode.size()) {noStLevel = true; break;}
				//cout << "i="<< i << endl;
				switch(heightTrop.number)
				{
					case 88 ://Цикл по тропапаузе code = 88
					{
						//cout << heightTrop.number << "\n";
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
						//cout << heightTrop.number << "\n";
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
						//cout << heightTrop.number << "\n";
						base.radioData.getRadio(VectorCode.at(i));
						i +=1;
						///Время запуска запуска
						if(i >= VectorCode.size()) {noStLevel = true;}
						else
						{
							base.radioData.getTime(VectorCode.at(i));
						}
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
		for ( i =i_begin; i != i_end; ++i )
		{
			if(base.number == (*i).number )
			{
				(*i).info = true;
				(*i).TTAA = base;		
				break;
			}
		}
	}
	catch(...)
	{
		cerr << "Error TTAA " << strCode <<  endl;
		// Error::no_ofErrors++;
	}
}