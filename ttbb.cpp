#include "WriteFile.h"

void recording_and_write::TTBB( char * code,  int key )
{
	TTBB_Database base;
	base.code_ = code;
	code = deleteEndl(code);
	base.information = true;
	bool theEnd = false;					// Çàâåðøåíèå ïðîãðàììû
	int step = 1;							// Øàã âûïîëíåíèÿ ïðîãðàììû	
	while ((*code) != '\0')
	{
		switch (step)
		{
		case 1 : // Íàõîæäåíèå âðåìåíè è äàòû çàïóñêà çîíäà
			{
				code = DateTime(code,base.memory);
				step++;
				break;
			}
		case 2:// Íàõîäæåíèå áîëüøîãî ðàéîíà è ñòàíöèè
			{
				code = DistrictStation(code, base.number);
				step++;
			}
		case 3:
			{
				step++;
				if( strstr( code, "N" ) == NULL ) //Ïðîâåðêà íà îòñóòñòâèå äàííûõ
				{
					int GGPPP = strtol(code, &code, 10);
					while( (*code) != '\0' )
					{
						Temp_Base new_level;
						if ((*code) == '/') 
						{
							code +=3;
							new_level.number = GGPPP ;
						} 
						else
						{
							new_level.number = GGPPP / 1000;
							new_level.pressure = GGPPP % 1000; 
						}
						new_level.information = true;	
						int i=0;
						code = TempDewpoint(code, new_level.info_temp,i);
 						base.level.push_back(new_level);
						GGPPP = strtol(code, &code, 10);
						if (GGPPP == 21212)
						{
							break;
						}
						
					}
				}
				else
				{
					theEnd = true;
					base.information = false;
				}

				break;
			}
		case 4 :
			{
				int GGPPP = strtol(code, &code, 10);
				while( (*code) != '\0' )
				{
					Wind_Base old_wind;
					if ((*code) == '/') 
					{
						code +=3;
						old_wind.number = GGPPP ;
						old_wind.information = false;
						old_wind.pressure = 999;

					} 
					else
					{
						old_wind.information = true;
						old_wind.number = GGPPP / 1000;
						old_wind.pressure = GGPPP % 1000; 
					}
					int i =0;
					code = Wind( code, old_wind.wind ,i);
					base.level_wind.push_back(old_wind);
					GGPPP = strtol(code, &code, 10);
					if (GGPPP == 31313 || GGPPP == 51515)
					{
						break;
					}
				}
				theEnd = true;
				step++;
				break;
			}	
		case 5 :
			{
				step++;
				//case 4 : 31313 èëè 41414
			}
		default:
			{
				theEnd = true;
				break;
			}
			
		}
		if (theEnd == true)
			break;
	}
	// Заполение списка ТТBB данными
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
