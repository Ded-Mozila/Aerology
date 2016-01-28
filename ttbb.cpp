#include "WriteFile.h"
int k = 0; 

void  recording_and_write::TTBB( string code, string strCode,  int key )
{
	try
	{
		//cout << strCode << endl;
		bool wind_node = false;
		vector<string> VectorCode(stringToVectorString(code));
		TTBB_Database base(strCode,DateTime(VectorCode[0],wind_node),DistrictStation(VectorCode[1]));
		if( strstr( VectorCode.at(2).c_str(), "N" ) == NULL )
		{ 
			int i = 2;
			bool noStLevel = false;
			//Цикл по температурным уровням
			while(i != VectorCode.size() || noStLevel == false)
			{
				Temp_Base new_level(VectorCode.at(i));
				i+=1;
				if(i >= VectorCode.size()) {noStLevel = true;}
				new_level.info_temp = TempDewpoint(VectorCode.at(i));
				
				base.level.push_back(new_level);//Добавление в список уровня
				i+=1;
				if(i >= VectorCode.size()) {noStLevel = true; break;}
				int GGPPP = atoi(VectorCode.at(i).c_str());
				if (GGPPP == 21212)
				{
					i+=1;
					break;
				}
			}
			noStLevel = false;
			//Цикл по уровням по ветру
			while(i != VectorCode.size() || noStLevel == false)
			{
				Wind_Base new_level(VectorCode.at(i));
				i+=1;
				if(i >= VectorCode.size()) {noStLevel = true;}
				new_level.wind = Wind(VectorCode.at(i),wind_node);
				
				base.level_wind.push_back(new_level);//Добавление в список уровня
				i+=1;
				if(i >= VectorCode.size()) {noStLevel = true; break;}
				int GGPPP = atoi(VectorCode.at(i).c_str());
				if (GGPPP == 31313)
				{
					noStLevel = true;
					break;
				}
			}
			noStLevel = false;
			//Цикл для дополнительной информации
			while(noStLevel == false )
			{
				if(i >= VectorCode.size()) {noStLevel = true;break;}
				switch(atoi(VectorCode.at(i).c_str()))
				{
					case 31313:
					{
						i+=1;	
						if(i >= VectorCode.size()) {noStLevel = true;}
						//cout << VectorCode.at(i) << "\n";
						base.radioData.getRadio(VectorCode.at(i));
						i +=1;
						///Время запуска запуска
						if(i >= VectorCode.size()) {noStLevel = true;}
						else
						{
							base.radioData.getTime(VectorCode.at(i));
							i+=1;	
						}
						//noStLevel = true;
						break;
					}
					case 41414:
					{
						i+=1;
						if(i >= VectorCode.size()) {noStLevel = true;}
						else
							base.cloud.getCloud(VectorCode.at(i));
						noStLevel = true;
						break;
					}
					default:
					{
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
		// Заполение списка ТТBB или ТТDD данными
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
				if(key == 1)
					(*i).TTBB = base;
				else (*i).TTDD = base;
				break;
			}
		}
	}
	catch(...)
	{
		cout << "TTBB " <<key<< " " << strCode <<  endl;
	}
}

